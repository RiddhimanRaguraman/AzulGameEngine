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
	//   [GAME_COMPONENT_BASE, 64)         -> reserved for GAME/app components
	// All components (incl. behavior components like RotateComponent) now live in
	// the engine DLL, so every id below is an engine id; the game range stays
	// reserved as an extension point should the app ever add its own component.
	enum ComponentId : unsigned int
	{
		COMPONENT_TRANSFORM = 0,
		COMPONENT_RENDER,
		COMPONENT_HIERARCHY,
		COMPONENT_LIGHT,
		COMPONENT_GPU_SKIN,
		COMPONENT_ANIM_CLIP,
		COMPONENT_ANIM_BLEND,
		COMPONENT_ROTATE,
		COMPONENT_SPRITE2D,
		COMPONENT_TEXT,

		ENGINE_COMPONENT_COUNT,   // engine ids must stay below GAME_COMPONENT_BASE

		GAME_COMPONENT_BASE = 32
	};

	static_assert(ENGINE_COMPONENT_COUNT <= GAME_COMPONENT_BASE,
		"too many engine components -- they would overrun the game id range");
}

#endif

// --- End of File ---
