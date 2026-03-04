//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "BoundingSphere.h"
#include "MeshLayout.h"
#include <vector>

using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertMesh(const char* const pFileName, const char* const pTargetName)
	{
		bool status;
		tinygltf::Model gltfModel;

		Trace::out("%-25s", pFileName);

		// Strip the extension .tga off the name
		unsigned int len = (uint32_t)strlen(pFileName);
		char* pTmp = new char[len + 1]();
		memset(pTmp, 0, len);
		memcpy(pTmp, pFileName, len - strlen(".glb"));

		// base name
		std::string BaseName = pTargetName;
		delete[] pTmp;

		// Load the gltfModel
		status = GLTF::Load(gltfModel, pFileName);
		assert(status);

		// Read glb into memory (raw buffer)
		char* poBuff = nullptr;
		unsigned int BuffSize(0);

		status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
		assert(status);

		// Get GLB_Header
		GLB_header glbHeader;
		status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
		assert(status);

		// Get Raw JSON
		char* poJSON = nullptr;
		unsigned int JsonSize(0);
		status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
		assert(status);

		// Get the Binary Buffer Address
		char* pBinaryBuff = nullptr;
		unsigned int BinaryBuffSize = 0;
		status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
		assert(status);

		// First pass: gather all vertex positions across submeshes to compute a single bounding sphere
		Sphere globalSphere;
		{
			std::vector<Vec3> allVerts;
			for (int i = 0; i < gltfModel.meshes.size(); ++i)
			{
				vboData vbo_pos;
				GLTF::SetVBO(gltfModel, "POSITION", vbo_pos, pBinaryBuff, i);
				VertexPos* vertices = (VertexPos*)(vbo_pos.poData);
				for (unsigned int j = 0; j < vbo_pos.count; ++j)
				{
					allVerts.emplace_back(vertices[j].Position.x, vertices[j].Position.y, vertices[j].Position.z);
				}
			}
			if (!allVerts.empty())
			{
				RitterSphere(globalSphere, allVerts.data(), (unsigned int)allVerts.size());
				Trace::out("\n[GLOBAL SPHERE] cntr:%f %f %f  rad:%f\n\n",
					globalSphere.cntr.x(), globalSphere.cntr.y(), globalSphere.cntr.z(), globalSphere.rad);
			}
			else
			{
				globalSphere.cntr.set(0.0f, 0.0f, 0.0f);
				globalSphere.rad = 0.0f;
			}
		}

		// Fill out the runtime Model data
		for (int i = 0; i < gltfModel.meshes.size(); ++i)
		{
			// runtime model
			meshData Mesh;

			// Model Name
			std::string modelName = BaseName;
			if (gltfModel.meshes.size() > 1)
			{
				modelName += std::to_string(i);
			}

			memset(Mesh.pMeshName, 0, meshData::FILE_NAME_SIZE);
			const size_t copyLen = std::min(modelName.size(), (size_t)meshData::FILE_NAME_SIZE - 1);
			memcpy_s(Mesh.pMeshName, meshData::FILE_NAME_SIZE, modelName.c_str(), copyLen);


			// Set vbo 
			GLTF::SetVBO(gltfModel, "POSITION", Mesh.vbo_vert, pBinaryBuff, i);
			GLTF::SetVBO(gltfModel, "NORMAL", Mesh.vbo_norm, pBinaryBuff, i);
			GLTF::SetVBO(gltfModel, "TEXCOORD_0", Mesh.vbo_uv, pBinaryBuff, i);
			GLTF::SetVBO(gltfModel, "COLOR_0", Mesh.vbo_color, pBinaryBuff, i);
			GLTF::SetVBO_index(gltfModel, "INDICES", Mesh.vbo_index, pBinaryBuff, i);

			// PolyCount
			Mesh.triCount = Mesh.vbo_index.count / 3;
			Mesh.vertCount = Mesh.vbo_vert.count;

			// Bounding Sphere (use global sphere across all submeshes)
			Mesh.sphereRadius = globalSphere.rad;
			Mesh.sphereCenter[0] = globalSphere.cntr.x();
			Mesh.sphereCenter[1] = globalSphere.cntr.y();
			Mesh.sphereCenter[2] = globalSphere.cntr.z();
			// RenderMode
			Mesh.mode = meshDataConverter::GetMode(gltfModel.meshes[i].primitives[0].mode);

			// Use the material referenced by this mesh's primitive
			int matIndex = gltfModel.meshes[i].primitives[0].material;
			if (matIndex >= 0 && matIndex < (int)gltfModel.materials.size())
			{
				GLTF::SetTexture(gltfModel, "Color", Mesh.text_color, pBinaryBuff, matIndex);
			}


			meshData_proto mA_proto;
			Mesh.Serialize(mA_proto);

			// -------------------------------
			//  Write to file
			//--------------------------------

			File::Handle fh;
			File::Error err;

			const char* pProtoFileType = nullptr;
			status = GLTF::GetAzulProtoType(pProtoFileType, Mesh);
			assert(status);
			std::string k = "";
			if (gltfModel.meshes.size() > 1)
				k = std::to_string(i);
			std::string OutputFileName = BaseName + k + pProtoFileType + ".proto.azul";
			Trace::out("\t\t --> %s\n", OutputFileName.c_str());

			err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
			assert(err == File::Error::SUCCESS);

			std::string strOut;
			mA_proto.SerializeToString(&strOut);

			File::Write(fh, strOut.data(), (uint32_t)strOut.length());
			assert(err == File::Error::SUCCESS);

			err = File::Close(fh);
			assert(err == File::Error::SUCCESS);

			// -------------------------------
			// Read and recreate model data
			// -------------------------------

			unsigned char* poNewBuff;
			unsigned int newBuffSize;

			err = File::GetFileAsBuffer(OutputFileName.c_str(), poNewBuff, newBuffSize);
			assert(err == File::Error::SUCCESS);

			std::string strIn((const char*)poNewBuff, newBuffSize);
			meshData_proto mB_proto;

			mB_proto.ParseFromString(strIn);

			meshData mB;
			mB.Deserialize(mB_proto);



			delete[] poNewBuff;
		}
		delete[] poBuff;
		delete[] poJSON;
	}
}

// --- End of File ---
