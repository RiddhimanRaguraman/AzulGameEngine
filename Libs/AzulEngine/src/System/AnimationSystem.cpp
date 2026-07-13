#include "AnimationSystem.h"
#include "World.h"
#include "AnimClipComponent.h"
#include "Anim.h"
#include "TimerController.h"
#include "ComputeBlend_OneAnim.h"

namespace Azul
{
	AnimationSystem::~AnimationSystem()
	{
	}

	void AnimationSystem::Update(World &world, AnimTime tDelta)
	{
		ComponentPool<AnimClipComponent> &pool = world.Pool<AnimClipComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			AnimClipComponent &c = pool.GetData(i);
			assert(c.pTimer);
			assert(c.pAnim);
			assert(c.pBlend);

			// (was AnimController_OneAnim::Update)
			c.pTimer->Update(c.ratio * tDelta);
			c.pBlend->AnimateMixerA(c.pAnim->GetClip(), c.pTimer->GetCurrTime());
		}
	}
}

// --- End of File ---
