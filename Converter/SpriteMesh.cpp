//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SpriteMesh.h"
#include "meshData.h"
#include "GLTF.h"
#include "File.h"
#include "MeshLayout.h"

using namespace tinygltf;

namespace Azul
{
	uint32_t g_SpriteIndices[] =
	{
		0, 2, 1,
		2, 0, 3
	};

	VertexPos g_SpriteVertices_pos[] =
	{
		// Triangle 0
		Vec3f(-0.5f,  0.5f, 0.0f), // 0
		Vec3f( 0.5f,  0.5f, 0.0f), // 1
		Vec3f( 0.5f, -0.5f, 0.0f), // 2

		// Triangle 1
		Vec3f(-0.5f, -0.5f, 0.0f) // 3
	};

	VertexTexCoord g_SpriteVertices_texCoord[] =
	{
		// Triangle 0
		Vec2f(0.0f, 0.0f), // 0
		Vec2f(1.0f, 0.0f), // 1
		Vec2f(1.0f, 1.0f), // 2

		// Triangle 1
		Vec2f(0.0f, 1.0f)  // 3

	};

	void CreateSpriteMesh()
	{
		// runtime model
		meshData  runModel;

		// Name
		std::string BaseName = "SpriteMesh";

		Trace::out("%-25s", BaseName.c_str());

		unsigned int numTris = (sizeof(g_SpriteIndices) / sizeof(g_SpriteIndices[0])) / 3;
		unsigned int numVerts = sizeof(g_SpriteVertices_pos) / sizeof(g_SpriteVertices_pos[0]);

		//---------------------------------
		// Model Name
		//---------------------------------
		const char *pMeshName = "SpriteMesh";
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pMeshName, strlen(pMeshName));

		//---------------------------------
		// Set vbo 
		//---------------------------------
		GLTF::SetCustomVBO(runModel.vbo_vert,
						   g_SpriteVertices_pos,
						   sizeof(g_SpriteVertices_pos),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC3,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_vert.Print("vbo_vert");

		GLTF::SetCustomVBO(runModel.vbo_uv,
						   g_SpriteVertices_texCoord,
						   sizeof(g_SpriteVertices_texCoord),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC2,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_uv.Print("vbo_uv");

		GLTF::SetCustomVBO_index( runModel.vbo_index,
								 g_SpriteIndices,
								 sizeof(g_SpriteIndices),
								 sizeof(g_SpriteIndices) / sizeof(g_SpriteIndices[0]),
								 vboData::VBO_COMPONENT::UNSIGNED_INT,
								 vboData::VBO_TYPE::SCALAR,
								 vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER
		);
		//runModel.vbo_index.Print("vbo_index");

		//--------------------------------
		// PolyCount
		//--------------------------------
		runModel.triCount = numTris;
		runModel.vertCount = numVerts;

		meshData_proto mA_proto;
		runModel.Serialize(mA_proto);

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;
		bool status;

		// Create output name
		const char *pProtoFileType = nullptr;
		status = GLTF::GetAzulProtoType(pProtoFileType, runModel);
		assert(status);

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";
		Trace::out(" --> %+30s\n", OutputFileName.c_str());

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), (DWORD)strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		// -------------------------------
		// Read and recreate model data
		// -------------------------------

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::END, 0);
		assert(err == File::Error::SUCCESS);

		DWORD FileLength;
		err = File::Tell(fh, FileLength);
		assert(err == File::Error::SUCCESS);

		char *poNewBuff = new char[FileLength]();
		assert(poNewBuff);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, poNewBuff, FileLength);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		std::string strIn(poNewBuff, FileLength);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);

		delete[] poNewBuff;

	}


}

// --- End of File ---
