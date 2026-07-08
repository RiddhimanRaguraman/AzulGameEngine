//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ANIM_CLIP_COMPONENT_H
#define AZUL_ANIM_CLIP_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class AnimController;

	// A single-clip animation being sampled each frame. Holds a NON-owning
	// AnimController* (a one-anim controller: timer + clip + mixer target).
	// Ownership stays with AnimMan's AnimNode, which deletes the controller --
	// mirrors GpuSkinComponent's non-owning ComputeBlend*. The AnimationSystem
	// drives pController->Update(tDelta) over the pool, replacing AnimMan::Update's
	// per-node walk for the one-anim path.
	//
	// One AnimClipComponent lives on a dedicated per-controller entity (NOT the
	// skin entity), so a controller driving several skin meshes is still sampled
	// exactly once per frame.
	struct AnimClipComponent
	{
		AnimController *pController;

		static const unsigned int kTypeId = COMPONENT_ANIM_CLIP;
	};
}

#endif

// --- End of File ---
