//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_COMPONENT_ID_H
#define GAME_COMPONENT_ID_H

#include "ComponentId.h"   // engine ids + GAME_COMPONENT_BASE

namespace Azul
{
	// Game/app component type ids. These live in the reserved game range
	// [GAME_COMPONENT_BASE, World::MAX_COMPONENT_TYPES) so they never collide
	// with engine components. Add new game behaviors here.
	enum GameComponentId : unsigned int
	{
		COMPONENT_ROTATE = GAME_COMPONENT_BASE + 0,

		GAME_COMPONENT_LAST   // keep < World::MAX_COMPONENT_TYPES (64)
	};
}

#endif

// --- End of File ---
