//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SkinningSystem.h"
#include "World.h"
#include "GpuSkinComponent.h"
#include "ComputeBlend.h"

namespace Azul
{
	SkinningSystem::~SkinningSystem()
	{
	}

	void SkinningSystem::Update(World &world, AnimTime tCurr)
	{
		AZUL_UNUSED_VAR(tCurr);

		ComponentPool<GpuSkinComponent> &pool = world.Pool<GpuSkinComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			GpuSkinComponent &skin = pool.GetData(i);
			assert(skin.pBlend);
			skin.pBlend->Execute();
		}
	}
}

// --- End of File ---
