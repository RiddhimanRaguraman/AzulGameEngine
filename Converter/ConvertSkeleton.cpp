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

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertSkeleton(const char *const pFileName, const char *const pTargetName)
	{
		assert(pFileName);
		Trace::out("%-25s", pFileName);

		bool status;
		tinygltf::Model gltfModel;

		// Load the gltfModel
		status = GLTF::Load(gltfModel, pFileName);
		assert(status);

		// SKELETON
		SKEL skelData;
		skelData.CreateSkeleton(gltfModel, pTargetName);

	}
}

// --- End of File ---
