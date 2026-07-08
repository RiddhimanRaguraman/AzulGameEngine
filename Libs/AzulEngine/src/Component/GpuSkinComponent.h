//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_GPU_SKIN_COMPONENT_H
#define AZUL_GPU_SKIN_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class ComputeBlend;

	// Handle to the GPU compute-skinning pipeline for a skinned entity. Holds a
	// NON-owning ComputeBlend* (ownership stays with the AnimController, per the
	// "components hold handles, not GPU objects" rule). The SkinningSystem drives
	// pBlend->Execute() over the pool -- replacing the pBlend->Execute() call that
	// used to live at the tail of GameObjectAnimSkin::Update.
	struct GpuSkinComponent
	{
		ComputeBlend *pBlend;

		static const unsigned int kTypeId = COMPONENT_GPU_SKIN;
	};
}

#endif

// --- End of File ---
