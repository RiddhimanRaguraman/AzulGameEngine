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
	void ConvertHierarchy(const char *const pFileName, const char *const pTargetName)
	{
		assert(pFileName);
		char sBuff[40];
		sprintf_s(sBuff, 40, "%s", pFileName);
		Trace::out("%-25s", sBuff);

		bool status;
		tinygltf::Model gltfModel;

		// Load the gltfModel
		status = GLTF::Load(gltfModel, pFileName);
		assert(status);

		// SKELETON
		SKEL skelData;
		skelData.CreateHierarchy(gltfModel, pTargetName);
	}
}

// --- End of File ---
