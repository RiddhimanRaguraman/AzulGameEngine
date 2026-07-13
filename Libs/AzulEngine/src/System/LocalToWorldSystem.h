#ifndef AZUL_LOCAL_TO_WORLD_SYSTEM_H
#define AZUL_LOCAL_TO_WORLD_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Computes world = Scale * Rot * Trans for every TransformComponent, in one
	// cache-friendly pass over the pool. Behavior systems that run after it (e.g.
	// RotateSystem) may overwrite the world for their entities.
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
