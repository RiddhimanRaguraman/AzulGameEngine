#ifndef AZUL_ANIM_CLIP_COMPONENT_H
#define AZUL_ANIM_CLIP_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class Anim;
	class TimerController;
	class ComputeBlend_OneAnim;

	// A single-clip animation being sampled each frame. Plain data -- NON-owning
	// views of resources owned by AnimMan's AnimNode (which deletes them). The
	// AnimationSystem advances pTimer by ratio*dt and samples pAnim's clip into
	// pBlend's mixer key buffers.
	//
	// One AnimClipComponent lives on a dedicated per-animation entity (NOT the
	// skin entity), so a controller driving several skin meshes is still sampled
	// exactly once per frame.
	struct AnimClipComponent
	{
		Anim *pAnim;
		TimerController *pTimer;
		ComputeBlend_OneAnim *pBlend;
		float ratio;

		static const unsigned int kTypeId = COMPONENT_ANIM_CLIP;
	};
}

#endif

// --- End of File ---
