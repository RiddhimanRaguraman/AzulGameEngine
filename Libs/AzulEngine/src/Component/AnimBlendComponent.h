//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ANIM_BLEND_COMPONENT_H
#define AZUL_ANIM_BLEND_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class AnimController;

	// A two-clip blended animation being sampled each frame. Holds a NON-owning
	// AnimController* (a two-anim controller: two timers + two clips + MixerC
	// blend). Ownership stays with AnimMan's AnimNode, which deletes it -- same
	// non-owning-handle shape as AnimClipComponent/GpuSkinComponent. The
	// BlendSystem drives pController->Update(tDelta) over the pool.
	//
	// The blend ratio itself (SPACE-key ramp) is still pushed separately via
	// AnimMan::BlendAnimation -> controller SetBlendTs; this component only drives
	// the per-frame sampling.
	struct AnimBlendComponent
	{
		AnimController *pController;

		static const unsigned int kTypeId = COMPONENT_ANIM_BLEND;
	};
}

#endif

// --- End of File ---
