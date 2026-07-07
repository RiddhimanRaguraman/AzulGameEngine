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
	enum ComponentId : unsigned int
	{
		COMPONENT_TRANSFORM = 0,
		COMPONENT_RENDER,
		COMPONENT_HIERARCHY,
		COMPONENT_LIGHT,

		COMPONENT_COUNT
	};
}

#endif

// --- End of File ---
