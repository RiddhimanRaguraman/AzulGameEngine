//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ECS_WORLD_MAN_H
#define AZUL_ECS_WORLD_MAN_H

#include "EngineDLLInterface.h"

namespace Azul
{
	class World;

	// Single engine-wide ECS World. Phase 2 bridge: GameObject creates its
	// entity here in its constructor and destroys it in its destructor, so the
	// object model and the ECS stay in lock-step during the migration.
	class AZUL_ENGINE_LIBRARY_API WorldMan
	{
	public:
		WorldMan() = delete;
		WorldMan(const WorldMan &) = delete;
		WorldMan &operator=(const WorldMan &) = delete;

		static World &GetWorld();
	};
}

#endif

// --- End of File ---
