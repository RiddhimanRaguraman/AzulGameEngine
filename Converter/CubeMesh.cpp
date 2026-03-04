//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CubeMesh.h"
#include "meshData.h"
#include "GLTF.h"
#include "File.h"
#include "MeshLayout.h"

using namespace tinygltf;

namespace Azul
{
	uint32_t g_CubeIndices[] =
	{
		1, 0,2 ,
		4, 3,5 ,
		7, 6,8 ,
		10,9,11 ,
		13,12,14 ,
		16,15,17 ,
		19,18,20 ,
		22,21,23 ,
		25,24,26 ,
		28,27,29 ,
		31,30,32 ,
		34,33,35
	};

	VertexPos g_CubeVertices_pos[] =
	{
		// Triangle 0
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(0.25f, -0.25f,  -0.25f),

		// Triangle 1
		Vec3f(0.25f,  -0.25f, -0.25f),
		Vec3f(0.25f,   0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f),

		// Triangle 2
		Vec3f(0.25f, -0.25f, -0.25f),
		Vec3f(0.25f, -0.25f,  0.25f),
		Vec3f(0.25f,  0.25f, -0.25f),

		// Triangle 3
		Vec3f(0.25f, -0.25f,  0.25f),
		Vec3f(0.25f,  0.25f,  0.25f),
		Vec3f(0.25f,  0.25f, -0.25f),

		// Triangle 4
		Vec3f(0.25f,  -0.25f,  0.25f),
		Vec3f(-0.25f,  -0.25f,  0.25f),
		Vec3f(0.25f,   0.25f,  0.25f),

		// Triangle 5
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),
		Vec3f(0.25f,   0.25f,  0.25f),

		// Triangle 6
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),

		// Triangle 7
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),

		// Triangle 8
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(0.25f, -0.25f,   0.25f),
		Vec3f(0.25f, -0.25f,  -0.25f),

		// Triangle 9
		Vec3f(0.25f, -0.25f,  -0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f, -0.25f,  0.25f),

		// Triangle 10
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(0.25f,  0.25f,  -0.25f),
		Vec3f(0.25f,  0.25f,   0.25f),

		// Triangle 11
		Vec3f(0.25f,  0.25f,   0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f)

	};

	VertexTexCoord g_CubeVertices_texCoord[] =
	{
		// Triangle 0
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),

		// Triangle 1
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 2
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 3
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 4
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 5
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 6
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 7
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 8
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),

		// Triangle 9
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 10
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 11
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f)

	};

	VertexNorm g_CubeVertices_norm[] =
	{
		// Triangle 0
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(0.6f,  -0.6f, -0.6f),

		// Triangle 1
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		// Triangle 2
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),

		// Triangle 3
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f,  0.6f,  0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),

		// Triangle 4
		Vec3f(0.6f, -0.6f, 0.6f),
		Vec3f(-0.6f, -0.6f, 0.6f),
		Vec3f(0.6f,  0.6f, 0.6f),

		// Triangle 5
		Vec3f(-0.6f, -0.6f, 0.6f),
		Vec3f(-0.6f,  0.6f, 0.6f),
		Vec3f(0.6f,  0.6f, 0.6f),

		// Triangle 6
		Vec3f(-0.6f, -0.6f,  0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f,  0.6f),

		// Triangle 7
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f,  0.6f),

		// Triangle 8
		Vec3f(-0.6f, -0.6f,  0.6f),
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f, -0.6f, -0.6f),

		// Triangle 9
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f,  0.6f),

		// Triangle 10
		Vec3f(-0.6f, 0.6f, -0.6f),
		Vec3f(0.6f, 0.6f, -0.6f),
		Vec3f(0.6f, 0.6f,  0.6f),

		// Triangle 11
		Vec3f(0.6f, 0.6f,  0.6f),
		Vec3f(-0.6f, 0.6f,  0.6f),
		Vec3f(-0.6f, 0.6f, -0.6f)

	};

	VertexColor g_CubeVertices_color[] =
	{
		 Vec4f(0.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(0.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(0.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(0.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f),
		 Vec4f(1.0f, 1.0f, 1.0f, 1.0f),
		 Vec4f(0.0f, 1.0f, 0.0f, 1.0f),
		 Vec4f(1.0f, 0.0f, 1.0f, 1.0f)

	};


	void CreateCubeMesh()
	{
		// runtime model
		meshData  runModel;

		// Name
		std::string BaseName = "CubeMesh";

		Trace::out("%-25s", BaseName.c_str());

		unsigned int numTris = (sizeof(g_CubeIndices) / sizeof(g_CubeIndices[0])) / 3;
		unsigned int numVerts = sizeof(g_CubeVertices_pos) / sizeof(g_CubeVertices_pos[0]);

		//---------------------------------
		// Model Name
		//---------------------------------
		const char* pMeshName = "CubeMesh";
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pMeshName, strlen(pMeshName));

		//---------------------------------
		// Set vbo 
		//---------------------------------
		GLTF::SetCustomVBO(runModel.vbo_vert,
			g_CubeVertices_pos,
			sizeof(g_CubeVertices_pos),
			numVerts,
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_vert.Print("vbo_vert");

		GLTF::SetCustomVBO(runModel.vbo_norm,
			g_CubeVertices_norm,
			sizeof(g_CubeVertices_norm),
			numVerts,
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC3,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_norm.Print("vbo_norm");

		GLTF::SetCustomVBO(runModel.vbo_uv,
			g_CubeVertices_texCoord,
			sizeof(g_CubeVertices_texCoord),
			numVerts,
			vboData::VBO_COMPONENT::FLOAT,
			vboData::VBO_TYPE::VEC2,
			vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_uv.Print("vbo_uv");

		GLTF::SetCustomVBO_index(runModel.vbo_index,
			g_CubeIndices,
			sizeof(g_CubeIndices),
			sizeof(g_CubeIndices) / sizeof(g_CubeIndices[0]),
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
		const char* pProtoFileType = nullptr;
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


}

// --- End of File ---
