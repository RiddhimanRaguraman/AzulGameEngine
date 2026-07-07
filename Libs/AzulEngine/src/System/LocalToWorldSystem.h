//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_LOCAL_TO_WORLD_SYSTEM_H
#define AZUL_LOCAL_TO_WORLD_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Computes world = Scale * Rot * Trans for every TransformComponent, in one
	// cache-friendly pass over the pool (no virtual dispatch, no tree walk).
	// Behavior objects (prefab-driven / skinned) still overwrite world in their
	// own Update after this runs; plain objects rely entirely on this system.
	class AZUL_ENGINE_LIBRARY_API LocalToWorldSystem : public System
	{
	public:
		LocalToWorldSystem() = default;
		LocalToWorldSystem(const LocalToWorldSystem &) = delete;
		LocalToWorldSystem &operator=(const LocalToWorldSystem &) = delete;
		virtual ~LocalToWorldSystem();

		virtual void Update(World &world, AnimTime tCurr) override;
	};
}

#endif

// --- End of File ---
