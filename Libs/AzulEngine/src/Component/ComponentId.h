//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_COMPONENT_ID_H
#define AZUL_COMPONENT_ID_H

namespace Azul
{
	// Central component type-id registry. Each component struct sets its
	// static kTypeId to one of these, so ids are unique and identical across
	// the DLL and the app (compile-time constants, not a runtime counter --
	// see World.h for why that matters across the module boundary).
	//
	// ID SPLIT (World::MAX_COMPONENT_TYPES == 64 slots total):
	//   [0, GAME_COMPONENT_BASE)          -> ENGINE components (defined here)
	//   [GAME_COMPONENT_BASE, 64)         -> GAME/app components (defined
	//                                        app-side in GameComponentId.h)
	// So the game can add behavior components without editing the engine, and
	// engine + game ids never collide.
	enum ComponentId : unsigned int
	{
		COMPONENT_TRANSFORM = 0,
		COMPONENT_RENDER,
		COMPONENT_HIERARCHY,
		COMPONENT_LIGHT,

		ENGINE_COMPONENT_COUNT,   // engine ids must stay below GAME_COMPONENT_BASE

		GAME_COMPONENT_BASE = 32
	};

	static_assert(ENGINE_COMPONENT_COUNT <= GAME_COMPONENT_BASE,
		"too many engine components -- they would overrun the game id range");
}

#endif

// --- End of File ---
