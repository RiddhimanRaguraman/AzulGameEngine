//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BlendSystem.h"
#include "World.h"
#include "AnimBlendComponent.h"
#include "AnimController.h"

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
			AnimBlendComponent &blend = pool.GetData(i);
			assert(blend.pController);
			blend.pController->Update(tDelta);
		}
	}
}

// --- End of File ---
