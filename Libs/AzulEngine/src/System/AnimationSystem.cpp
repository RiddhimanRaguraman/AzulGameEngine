//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimationSystem.h"
#include "World.h"
#include "AnimClipComponent.h"
#include "AnimController.h"

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
			AnimClipComponent &clip = pool.GetData(i);
			assert(clip.pController);
			clip.pController->Update(tDelta);
		}
	}
}

// --- End of File ---
