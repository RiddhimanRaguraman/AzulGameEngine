//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "PyramidMesh.h"
#include "meshData.h"
#include "GLTF.h"
#include "File.h"
#include "MeshLayout.h"
#include "MathEngine.h"

using namespace tinygltf;

namespace Azul
{
	VertexPos g_PyramidVertices_pos[] =
	{
		Vec3f(-0.25f,   0.0f,  -0.25f  ),
		Vec3f(-0.25f,  -0.0f,  -0.25f  ),
		Vec3f( 0.25f,  -0.0f,  -0.25f  ),
		Vec3f(  0.0f,   1.0f,   -0.0f  ),
		Vec3f( 0.25f,  -0.0f,  -0.25f  ),
		Vec3f( 0.25f,  -0.0f,   0.25f  ),
		Vec3f(  0.0f,   1.0f,   -0.0f  ),
		Vec3f( 0.25f,   0.0f,   0.25f  ),
		Vec3f( 0.25f,  -0.0f,   0.25f  ),
		Vec3f(-0.25f,  -0.0f,   0.25f  ),
		Vec3f(  0.0f,   1.0f,    0.0f  ),
		Vec3f( -0.0f,   1.0f,    0.0f  ),
		Vec3f(-0.25f,  -0.0f,   0.25f  ),
		Vec3f(-0.25f,  -0.0f,  -0.25f  ),
		Vec3f( -0.0f,   1.0f,    0.0f  ),
		Vec3f( -0.0f,   1.0f,   -0.0f  ),
		Vec3f(-0.25f,  -0.0f,   0.25f  ),
		Vec3f( 0.25f,  -0.0f,   0.25f  ),
		Vec3f( 0.25f,  -0.0f,  -0.25f  ),
		Vec3f(-0.25f,  -0.0f,  -0.25f  ),
		Vec3f( -0.0f,   1.0f,   -0.0f  ),
		Vec3f(  0.0f,   1.0f,   -0.0f  ),
		Vec3f(  0.0f,   1.0f,    0.0f  ),
		Vec3f( -0.0f,   1.0f,    0.0f  )
	};

	uint32_t g_PyramidIndices[] =
	{
		1,  0,  2,
		3,  2,  0,
		5,  4,  6,
		7,  5,  6,
		9,  8, 10,
		11,  9, 10,
		13, 12, 14,
		15, 13, 14,
		17, 16, 18,
		19, 18, 16,
		21, 20, 22,
		23, 22, 20
	};

	VertexTexCoord g_PyramidVertices_texCoord[] =
	{
		Vec2f(  0.0f,  0.0f  ),
		Vec2f(  0.0f,  1.0f  ),
		Vec2f(  1.0f,  1.0f  ),
		Vec2f(  1.0f,  0.0f  ),
		Vec2f(  0.0f,  1.0f  ),
		Vec2f(  1.0f,  1.0f  ),
		Vec2f(  0.0f,  0.0f  ),
		Vec2f(  1.0f,  0.0f  ),
		Vec2f(  1.0f,  1.0f  ),
		Vec2f(  0.0f,  1.0f  ),
		Vec2f(  1.0f,  0.0f  ),
		Vec2f(  0.0f,  0.0f  ),
		Vec2f(  0.0f,  0.0f  ),
		Vec2f(  0.0f,  1.0f  ),
		Vec2f(  1.0f,  0.0f  ),
		Vec2f(  1.0f,  1.0f  ),
		Vec2f(  0.0f,  0.0f  ),
		Vec2f(  1.0f,  0.0f  ),
		Vec2f(  1.0f,  1.0f  ),
		Vec2f(  0.0f,  1.0f  ),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f)
	};

	VertexNorm g_PyramidVertices_norm[] =
	{
		Vec3f( -0.6f,   0.6f,  -0.6f  ),
		Vec3f( -0.6f,  -0.6f,  -0.6f  ),
		Vec3f(  0.6f,  -0.6f,  -0.6f  ),
		Vec3f(  0.6f,   0.6f,  -0.6f  ),
		Vec3f(  0.6f,  -0.6f,  -0.6f  ),
		Vec3f(  0.6f,  -0.6f,   0.6f  ),
		Vec3f(  0.6f,   0.6f,  -0.6f  ),
		Vec3f(  0.6f,   0.6f,   0.6f  ),
		Vec3f(  0.6f,  -0.6f,   0.6f  ),
		Vec3f( -0.6f,  -0.6f,   0.6f  ),
		Vec3f(  0.6f,   0.6f,   0.6f  ),
		Vec3f( -0.6f,   0.6f,   0.6f  ),
		Vec3f( -0.6f,  -0.6f,   0.6f  ),
		Vec3f( -0.6f,  -0.6f,  -0.6f  ),
		Vec3f( -0.6f,   0.6f,   0.6f  ),
		Vec3f( -0.6f,   0.6f,  -0.6f  ),
		Vec3f( -0.6f,  -0.6f,   0.6f  ),
		Vec3f(  0.6f,  -0.6f,   0.6f  ),
		Vec3f(  0.6f,  -0.6f,  -0.6f  ),
		Vec3f( -0.6f,  -0.6f,  -0.6f  ),
		Vec3f( -0.6f,   0.6f,  -0.6f  ),
		Vec3f(  0.6f,   0.6f,  -0.6f  ),
		Vec3f(  0.6f,   0.6f,   0.6f  ),
		Vec3f( -0.6f,   0.6f,   0.6f  )

	};


	void CreatePyramidMesh()
	{
		// runtime model
		meshData  runModel;

		// Name
		std::string BaseName = "PyramidMesh";

		Trace::out("%-25s", BaseName.c_str());

		unsigned int numTris = (sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0])) / 3;
		unsigned int numVerts = sizeof(g_PyramidVertices_pos) / sizeof(g_PyramidVertices_pos[0]);

		//---------------------------------
		// Model Name
		//---------------------------------
		const char *pMeshName = "PyramidMesh";
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pMeshName, strlen(pMeshName));


		// Correct make Z dominate, correct postion
		Rot M(Rot1::X, MATH_PI2);
		for(size_t i = 0; i < numVerts; i++)
		{
			Vec4 v(g_PyramidVertices_pos[i].Position.x, 
				   g_PyramidVertices_pos[i].Position.y, 
				   g_PyramidVertices_pos[i].Position.z,
				   1.0f);

			v = v * M;
			g_PyramidVertices_pos[i].Position.x = v[x];
			g_PyramidVertices_pos[i].Position.y = v[y];
			g_PyramidVertices_pos[i].Position.z = v[z];

			//Trace::out("%d: %f %f %f \n", 
			//		   i, 
			//		   g_PyramidVertices_pos[i].Position.x, 
			//		   g_PyramidVertices_pos[i].Position.y,
			//		   g_PyramidVertices_pos[i].Position.z);
		}

		for(size_t i = 0; i < numVerts; i++)
		{
			Vec4 v(g_PyramidVertices_norm[i].Norm.x,
				   g_PyramidVertices_norm[i].Norm.y,
				   g_PyramidVertices_norm[i].Norm.z,
				   1.0f);

			v = v * M;

			g_PyramidVertices_norm[i].Norm.x = v[x];
			g_PyramidVertices_norm[i].Norm.y = v[y];
			g_PyramidVertices_norm[i].Norm.z = v[z];
		}



		//---------------------------------
		// Set vbo 
		//---------------------------------
		GLTF::SetCustomVBO(runModel.vbo_vert,
						   g_PyramidVertices_pos,
						   sizeof(g_PyramidVertices_pos),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC3,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_vert.Print("vbo_vert");

		GLTF::SetCustomVBO(runModel.vbo_norm,
						   g_PyramidVertices_norm,
						   sizeof(g_PyramidVertices_norm),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC3,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_norm.Print("vbo_norm");

		GLTF::SetCustomVBO(runModel.vbo_uv,
						   g_PyramidVertices_texCoord,
						   sizeof(g_PyramidVertices_texCoord),
						   numVerts,
						   vboData::VBO_COMPONENT::FLOAT,
						   vboData::VBO_TYPE::VEC2,
						   vboData::VBO_TARGET::ARRAY_BUFFER
		);
		//runModel.vbo_uv.Print("vbo_uv");

		GLTF::SetCustomVBO_index(runModel.vbo_index,
								 g_PyramidIndices,
								 sizeof(g_PyramidIndices),
								 sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0]),
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
		unsigned char *poNewBuff;
		unsigned int newBuffSize;

		err = File::GetFileAsBuffer(OutputFileName.c_str(), poNewBuff, newBuffSize);
		assert(err == File::Error::SUCCESS);

		std::string strIn((const char *)poNewBuff, newBuffSize);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);

		delete[] poNewBuff;

	}


}

// --- End of File ---
