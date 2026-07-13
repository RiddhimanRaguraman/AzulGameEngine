#ifndef AZUL_GPU_SKIN_COMPONENT_H
#define AZUL_GPU_SKIN_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class ComputeBlend;

	// Handle to the GPU compute-skinning pipeline for a skinned entity. Holds a
	// NON-owning ComputeBlend* (ownership stays with AnimMan's AnimNode, per the
	// "components hold handles, not GPU objects" rule). The SkinningSystem drives
	// pBlend->Execute() over the pool. AnimMan::privCreateSkinEntity adds this to
	// each skinned entity (there is no GameObject anymore).
	struct GpuSkinComponent
	{
		ComputeBlend *pBlend;

		static const unsigned int kTypeId = COMPONENT_GPU_SKIN;
	};
}

#endif

// --- End of File ---
