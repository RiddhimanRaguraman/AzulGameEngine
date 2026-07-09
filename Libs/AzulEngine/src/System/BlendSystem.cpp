//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BlendSystem.h"
#include "World.h"
#include "AnimBlendComponent.h"
#include "Anim.h"
#include "TimerController.h"
#include "ComputeBlend_TwoAnim.h"

namespace Azul
{
	BlendSystem::~BlendSystem()
	{
	}

	void BlendSystem::Update(World &world, AnimTime tDelta)
	{
		ComponentPool<AnimBlendComponent> &pool = world.Pool<AnimBlendComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			AnimBlendComponent &b = pool.GetData(i);
			assert(b.pTimerA);
			assert(b.pTimerB);
			assert(b.pAnimA);
			assert(b.pAnimB);
			assert(b.pBlend);

			// (was AnimController_TwoAnim::Update)
			b.pTimerA->Update(b.ratioA * tDelta);
			b.pBlend->AnimateMixerA(b.pAnimA->GetClip(), b.pTimerA->GetCurrTime());

			b.pTimerB->Update(b.ratioB * tDelta);
			b.pBlend->AnimateMixerB(b.pAnimB->GetClip(), b.pTimerB->GetCurrTime());

			b.pBlend->AnimateMixerC();
		}
	}
}

// --- End of File ---
