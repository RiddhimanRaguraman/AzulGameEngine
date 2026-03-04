//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "GLTF.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "MathEngine.h"

#include "ConvertSkin.h"

using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertSkin(const char* const pFileName, const char* const pTargetName)
	{
		bool status;
		tinygltf::Model gltfModel;

		// runtime model
		meshData  runModel;

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

		// --------------------------------------------------
		//  Fill: SkinMesh
		// --------------------------------------------------

		SkinMesh Skin0;

		// THis is hard code... need to fix
		size_t inverseMatrixAccessorIndex = (size_t)gltfModel.skins[0].inverseBindMatrices;
		size_t weightAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("WEIGHTS_0")->second;
		size_t jointAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("JOINTS_0")->second;
		size_t indexAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].indices;
		size_t posAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("POSITION")->second;
		size_t normsAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("NORMAL")->second;
		size_t texCoordAccessorIndex = (size_t)gltfModel.meshes[0].primitives[0].attributes.find("TEXCOORD_0")->second;


		unsigned char* pBuff = (unsigned char*)&gltfModel.buffers[0].data[0];

		// Joint
		auto JointAccessor = gltfModel.accessors[jointAccessorIndex];
		auto JointBuffView = gltfModel.bufferViews[(size_t)JointAccessor.bufferView];

		unsigned char* pJointBuff = pBuff + JointBuffView.byteOffset;
		assert(JointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
		assert(JointAccessor.type == TINYGLTF_TYPE_VEC4);

		// Weight
		auto WeightAccessor = gltfModel.accessors[weightAccessorIndex];
		auto WeightBuffView = gltfModel.bufferViews[(size_t)WeightAccessor.bufferView];

		unsigned char* pWeightBuff = pBuff + WeightBuffView.byteOffset;
		assert(WeightAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(WeightAccessor.type == TINYGLTF_TYPE_VEC4);

		// InverseMatrix
		auto InverseAccessor = gltfModel.accessors[inverseMatrixAccessorIndex];
		auto InverseBuffView = gltfModel.bufferViews[(size_t)InverseAccessor.bufferView];

		unsigned char* pInverseMatrixBuff = pBuff + InverseBuffView.byteOffset;
		assert(InverseAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(InverseAccessor.type == TINYGLTF_TYPE_MAT4);

		// Verify the joint count, weight count, vert count all the same
		assert(WeightAccessor.count == JointAccessor.count);

		// Index
		auto IndexAccessor = gltfModel.accessors[indexAccessorIndex];
		auto IndexBuffView = gltfModel.bufferViews[(size_t)IndexAccessor.bufferView];

		unsigned char* pIndexBuff = pBuff + IndexBuffView.byteOffset;
		assert(IndexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
		assert(IndexAccessor.type == TINYGLTF_TYPE_SCALAR);

		// Pos
		auto PosAccessor = gltfModel.accessors[posAccessorIndex];
		auto PosBuffView = gltfModel.bufferViews[(size_t)PosAccessor.bufferView];

		unsigned char* pPosBuff = pBuff + PosBuffView.byteOffset;
		assert(PosAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(PosAccessor.type == TINYGLTF_TYPE_VEC3);

		// Norms
		auto NormsAccessor = gltfModel.accessors[normsAccessorIndex];
		auto NormsBuffView = gltfModel.bufferViews[(size_t)NormsAccessor.bufferView];

		unsigned char* pNormsBuff = pBuff + NormsBuffView.byteOffset;
		assert(NormsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(NormsAccessor.type == TINYGLTF_TYPE_VEC3);

		// Tex Coord
		auto TexCoordAccessor = gltfModel.accessors[texCoordAccessorIndex];
		auto TexCoordBuffView = gltfModel.bufferViews[(size_t)TexCoordAccessor.bufferView];

		unsigned char* pTexCoordBuff = pBuff + TexCoordBuffView.byteOffset;
		assert(TexCoordAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(TexCoordAccessor.type == TINYGLTF_TYPE_VEC2);

		// --------------------------------------------------
		//  Fill: std::vector< Mat4 > invBone;
		// --------------------------------------------------
		//Trace::out("\n");
		//Trace::out("\n");
		//Trace::out("Matrix Inv (Row Major):\n");
		Mat4* pMat4 = (Mat4*)pInverseMatrixBuff;
		for (size_t i = 0; i < InverseAccessor.count; i++)
		{
			assert(pMat4);
			Skin0.invBone.push_back(pMat4[i]);

			/*char s[8];
			sprintf_s(s, 8, "%d: ", (uint32_t)i);
			pMat4[i].Print(s);*/
		}

		// -------------------------------------------------
		//  Joint table
		// -------------------------------------------------

	/*	Skin0.jointTable = gltfModel.skins[0].joints;
		Trace::out("\n");
		Trace::out("int JointTable_Size = %d; \n", Skin0.jointTable.size());
		Trace::out("int JointTable[] = { \n");
		for (size_t i = 0; i < Skin0.jointTable.size(); i++)
		{
			if (i != Skin0.jointTable.size() - 1)
			{
				Trace::out("\t\t\t\t%d, \n", Skin0.jointTable[i]);
			}
			else
			{
				Trace::out("\t\t\t\t%d }; \n", Skin0.jointTable[i]);
			}
		}
		Trace::out("\n");*/

		// --------------------------------------------------
		//  Fill: std::vector< iuVect4 > joints; 
		//  Fill: std::vector< fVect4 > weights;
		//  Fill: std::vector< fVect3 > pos;
		//  Fill: std::vector< fVect3 > norms;
		//  Fill: std::vector< fVect2 > text_coord;
		// --------------------------------------------------

		assert(PosAccessor.count == NormsAccessor.count);
		assert(PosAccessor.count == TexCoordAccessor.count);
		assert(PosAccessor.count == JointAccessor.count);
		assert(PosAccessor.count == WeightAccessor.count);

		Vec4si* pJointVect4 = (Vec4si*)pJointBuff;
		Vec4f* pWeightVect4 = (Vec4f*)pWeightBuff;
		Vec3f* pPos = (Vec3f*)pPosBuff;
		Vec3f* pNorms = (Vec3f*)pNormsBuff;
		Vec2f* pTexCoord = (Vec2f*)pTexCoordBuff;

		for (size_t i = 0; i < PosAccessor.count; i++)
		{
			Skin0.joints.push_back(Vec4ui((unsigned int)pJointVect4[i].x,
				(unsigned int)pJointVect4[i].y,
				(unsigned int)pJointVect4[i].z,
				(unsigned int)pJointVect4[i].w));
			Skin0.weights.push_back(pWeightVect4[i]);
			Skin0.pos.push_back(pPos[i]);
			Skin0.norms.push_back(pNorms[i]);
			Skin0.text_coord.push_back(pTexCoord[i]);


			/*Trace::out("[%d] w: %f %f %f %f  j: %d %d %d %d  \n",	i, pWeightVect4[i].x, pWeightVect4[i].y, pWeightVect4[i].z, pWeightVect4[i].w, 	pJointVect4[i].x, pJointVect4[i].y, pJointVect4[i].z, pJointVect4[i].w);*/


			//Trace::out("[%d] pos: %f %f %f   \n",
			//		   i,
			//		   pPos[i].x, pPos[i].y, pPos[i].z);

			// Note: color was skipped... no data
		}

		// --------------------------------------------------
		//  Fill:  std::vector< iuVect3 > index;
		// --------------------------------------------------

		// protection.. might need rework if model is in unsigned int
		//              larger than 64K vert models

		Vec3si* pVec3us = (Vec3si*)pIndexBuff;

		//Trace::out("\n");
		for (size_t i = 0; i < IndexAccessor.count / 3; i++)
		{
			Skin0.index.push_back(Vec3ui(pVec3us[i].x,
				pVec3us[i].y,
				pVec3us[i].z));

			//Trace::out("[%d] index  %d %d %d \n",i, pVec3us[i].x, pVec3us[i].y, pVec3us[i].z);
		}
		//Trace::out("\n");

		// --------------------------------------------------
		//  Fill: std::string skinName;
		//	Fill: size_t vertCount;
		//	Fill: size_t polyCount;
		// --------------------------------------------------

		Skin0.skinName = gltfModel.meshes[0].name;
		Skin0.vertCount = PosAccessor.count;
		Skin0.polyCount = IndexAccessor.count / 3;


		// -------------------------------------------------
		//  Fill Mesh Data protoBuff
		// -------------------------------------------------


		//---------------------------------
		// Model Name
		//---------------------------------
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, Skin0.skinName.c_str(), Skin0.skinName.size());

		//---------------------------------
		// Set vbo 
		//---------------------------------

		GLTF::SetCustomVBO(runModel.vbo_vert,
			(char*)&Skin0.pos[0],
			(uint32_t)Skin0.pos.size() * sizeof(Vec3f),
			(uint32_t)Skin0.pos.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);

		GLTF::SetCustomVBO(runModel.vbo_norm,
			(char*)&Skin0.norms[0],
			(uint32_t)Skin0.norms.size() * sizeof(Vec3f),
			(uint32_t)Skin0.norms.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);

		GLTF::SetCustomVBO(runModel.vbo_uv,
			(char*)&Skin0.text_coord[0],
			(uint32_t)Skin0.text_coord.size() * sizeof(Vec2f),
			(uint32_t)Skin0.text_coord.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC2,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);

		// ------------------------------------------------
		// COLOR 
		//      not in model, use default VBO (disabled)
		// ------------------------------------------------
		// GLTF::SetCustomVBO(
		//	                 runModel.vbo_color,
		//	                 (char *)&Skin0.color[0],
		//	                 Skin0.color.size() * sizeof(Vec4f),
		//	                 Skin0.color.size(),
		//	                 vboData::VBO_COMPONENT::FLOAT,
		//	                 vboData::VBO_TYPE::VEC4,
		//	                 vboData::VBO_TARGET::ARRAY_BUFFER
		//                   );
		// ------------------------------------------------

		GLTF::SetCustomVBO_index(runModel.vbo_index,
			(char*)&Skin0.index[0],
			(uint32_t)Skin0.index.size() * sizeof(Vec3ui),
			(uint32_t)Skin0.index.size() * 3,  //<--- 3 (8 hrs to find)
			vboData::VBO_COMPONENT::UNSIGNED_INT,
			vboData::VBO_TYPE::SCALAR,
			vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER
		);

		GLTF::SetCustomVBO(runModel.vbo_weights,
			(char*)&Skin0.weights[0],
			(uint32_t)Skin0.weights.size() * sizeof(Vec4f),
			(uint32_t)Skin0.weights.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC4,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);

		GLTF::SetCustomVBO(runModel.vbo_joints,
			(char*)&Skin0.joints[0],
			(uint32_t)Skin0.joints.size() * sizeof(Vec4ui),
			(uint32_t)Skin0.joints.size(),
			vboData::VBO_COMPONENT::UNSIGNED_INT,
			vboData::VBO_TYPE::VEC4,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);


		GLTF::SetCustomVBO(runModel.vbo_invBind,
			(char*)&Skin0.invBone[0],
			(uint32_t)Skin0.invBone.size() * sizeof(Mat4),
			(uint32_t)Skin0.invBone.size(),
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::MAT4,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);

		//--------------------------------
		// PolyCount
		//--------------------------------
		runModel.triCount = runModel.vbo_index.count / 3;
		runModel.vertCount = runModel.vbo_vert.count;

		//--------------------------------
		// RenderMode
		//--------------------------------
		runModel.mode = meshDataConverter::GetMode(gltfModel.meshes[0].primitives[0].mode);
		assert(runModel.mode == meshData::RENDER_MODE::MODE_TRIANGLES);

		//runModel.Print("mA");
		meshData_proto mA_proto;
		runModel.Serialize(mA_proto);

		//Trace::out("\n");
		//Trace::out("message size: %d \n", mA_proto.ByteSizeLong());
		//Trace::out("\n");

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;

		const char* pProtoFileType = nullptr;
		status = GLTF::GetAzulProtoType(pProtoFileType, runModel);
		assert(status);

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";
		Trace::out(" --> %+30s\n", OutputFileName.c_str());

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

		delete[] poJSON;
		delete[] poBuff;
		delete[] poNewBuff;

	}
}

// --- End of File ---