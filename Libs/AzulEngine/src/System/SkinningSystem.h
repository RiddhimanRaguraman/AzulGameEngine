//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_SKINNING_SYSTEM_H
#define AZUL_SKINNING_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Dispatches the GPU compute-skinning pipeline (MixerA interpolate +
	// WorldComputeA compose) for every skinned entity, in one flat pass over the
	// GpuSkinComponent pool. Replaces the pBlend->Execute() call formerly made
	// per-object at the tail of GameObjectAnimSkin::Update.
	//
	// Ordering: must run AFTER the AnimController updates have sampled the clips
	// into the mixer key buffers (AnimMan::Update, called before
	// GameObjectMan::Update in the scene). Execute() composes bone matrices in
	// model space and does NOT read the object's world matrix, so it is
	// independent of LocalToWorldSystem.
	class AZUL_ENGINE_LIBRARY_API SkinningSystem : public System
	{
	public:
		SkinningSystem() = default;
		SkinningSystem(const SkinningSystem &) = delete;
		SkinningSystem &operator=(const SkinningSystem &) = delete;
		virtual ~SkinningSystem();

		virtual void Update(World &world, AnimTime tCurr) override;
	};
}

#endif

// --- End of File ---
