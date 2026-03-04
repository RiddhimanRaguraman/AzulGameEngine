//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "ConvertAnim.h"
#include "MeshLayout.h"
#include "Anim.h"

using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertAnim(const char* const pFileName, const char* const pTargetName)
	{
		bool status;
		tinygltf::Model gltfModel;

		// runtime model
		meshData  runModel;

		Trace::out("%-25s ", pFileName);
		//Trace::out("\n");

		// Strip the extension .tga off the name
		unsigned int len2 = (uint32_t)strlen(pFileName);
		char* pTmp = new char[len2 + 1]();
		memset(pTmp, 0, len2);
		memcpy(pTmp, pFileName, len2 - strlen(".glb"));

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

		// ANIMATION
		ANIM animData;
		animData.CreateAnim(gltfModel, pTargetName);

		delete[] poBuff;
		delete[] poJSON;



	}
}

// --- End of File ---
