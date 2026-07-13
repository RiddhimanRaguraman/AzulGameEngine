#ifndef AZUL_ECS_ICOMPONENT_POOL_H
#define AZUL_ECS_ICOMPONENT_POOL_H

#include "EngineDLLInterface.h"
#include "Entity.h"

namespace Azul
{
	// Type-erased base so the World can hold ComponentPool<C> of different C
	// in one array and drive them polymorphically (e.g. when an entity is
	// destroyed every pool must drop that entity's component).
	class AZUL_ENGINE_LIBRARY_API IComponentPool
	{
	public:
		IComponentPool() = default;
		IComponentPool(const IComponentPool &) = delete;
		IComponentPool &operator=(const IComponentPool &) = delete;
		virtual ~IComponentPool();

		virtual void OnEntityDestroyed(const Entity &e) = 0;
	};
}

#endif

// --- End of File ---
