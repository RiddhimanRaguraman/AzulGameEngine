//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ECS_SYSTEM_H
#define AZUL_ECS_SYSTEM_H

#include "EngineDLLInterface.h"
#include "AnimTime.h"

namespace Azul
{
	class World;

	// Base for all systems: logic that runs each frame over the World's
	// component pools. A system iterates one pool's dense array and checks the
	// World for the other components it needs -- no lambdas, no view objects.
	class AZUL_ENGINE_LIBRARY_API System
	{
	public:
		System() = default;
		System(const System &) = delete;
		System &operator=(const System &) = delete;
		virtual ~System();

		virtual void Update(World &world, AnimTime tCurr) = 0;
	};
}

#endif

// --- End of File ---
