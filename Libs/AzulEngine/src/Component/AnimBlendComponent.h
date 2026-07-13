#ifndef AZUL_ANIM_BLEND_COMPONENT_H
#define AZUL_ANIM_BLEND_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class Anim;
	class TimerController;
	class ComputeBlend_TwoAnim;

	// A two-clip blended animation being sampled each frame. Plain data --
	// NON-owning views of resources owned by AnimMan's AnimNode (which deletes
	// them). The BlendSystem advances both timers, samples both clips (MixerA/B)
	// and blends (MixerC) into pBlend's key buffers.
	//
	// The blend ratio itself (SPACE-key ramp) is still pushed separately via
	// AnimMan::BlendAnimation -> pBlend->SetBlendTs; this component only drives
	// the per-frame sampling.
	struct AnimBlendComponent
	{
		Anim *pAnimA;
		TimerController *pTimerA;
		float ratioA;

		Anim *pAnimB;
		TimerController *pTimerB;
		float ratioB;

		ComputeBlend_TwoAnim *pBlend;

		static const unsigned int kTypeId = COMPONENT_ANIM_BLEND;
	};
}

#endif

// --- End of File ---
