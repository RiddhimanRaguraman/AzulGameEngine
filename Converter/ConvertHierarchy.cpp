//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "MathEngine.h"
#include "ConvertSkeleton.h"
#include "Skeleton.h"
#include "Anim.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertHierarchy(const char *const pName, const char * const pTargetName)
	{
		assert(pName);
		char sBuff[40];
		sprintf_s(sBuff, 40, "Hierarchy(%s)", pName);
		Trace::out("%-25s", sBuff);

		bool status;
		tinygltf::Model gltfModel;

		// runtime model
		meshData  runModel;

		// glb file name defined here:
		const char *pGLB_Name = pName;
		const char *pGLB_extension = ".glb";

		size_t FileNameSize = strlen(pGLB_Name) + strlen(pGLB_extension) + 1;

		char *poFileName = new char[FileNameSize]();
		strcat_s(poFileName, FileNameSize, pGLB_Name);
		strcat_s(poFileName, FileNameSize, pGLB_extension);

		// Load the gltfModel
		status = GLTF::Load(gltfModel, poFileName);
		assert(status);

		// SKELETON
		SKEL skelData;
		skelData.CreateHierarchy(gltfModel, pTargetName);


		delete[] poFileName;
	}
}

// --- End of File ---
