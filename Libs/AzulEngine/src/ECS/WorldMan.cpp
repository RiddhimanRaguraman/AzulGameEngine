//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WorldMan.h"
#include "World.h"

namespace Azul
{
	World &WorldMan::GetWorld()
	{
		// One World for the whole engine, created on first use. Explicit
		// Create/Destroy can replace this later if lifetime control is needed.
		static World world;
		return world;
	}
}

// --- End of File ---
