//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WorldMan.h"
#include "World.h"

namespace Azul
{
	World *WorldMan::posWorld = nullptr;

	World &WorldMan::GetWorld()
	{
		// Lazy: created on first use (e.g. the first GameObject), so callers
		// don't need an explicit Create(). Destroy() frees it during teardown.
		if (posWorld == nullptr)
		{
			posWorld = new World();
		}
		return *posWorld;
	}

	void WorldMan::Destroy()
	{
		// ~World frees every pool (and each pool's arrays) plus the generation
		// and free-list arrays. Runs during scene unload, before the leak check.
		delete posWorld;
		posWorld = nullptr;
	}
}

// --- End of File ---
