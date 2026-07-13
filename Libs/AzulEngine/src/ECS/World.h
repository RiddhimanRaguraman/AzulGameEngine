#ifndef AZUL_ECS_WORLD_H
#define AZUL_ECS_WORLD_H

#include "EngineDLLInterface.h"
#include "Entity.h"
#include "IComponentPool.h"
#include "ComponentPool.h"

namespace Azul
{
	// The ECS registry: owns entities (with generation recycling) and one
	// ComponentPool per component type, indexed by that type's explicit id.
	//
	// Component-id rule: each component struct declares
	//     static const unsigned int kTypeId = <n>;   // 0 .. MAX_COMPONENT_TYPES-1
	// Explicit compile-time ids -- NOT a runtime counter. A templated counter's
	// function-local static would be instantiated separately in the DLL and the
	// app and could hand back different ids for the same type, corrupting the
	// pool lookup. A constant is identical in every module.
	//
	// TEMPLATE EXCEPTION (WhatToDo.md 0a): the per-type Add/TryGet/Has/Pool are
	// inherently generic and header-only (templates aren't dll-exported). The
	// non-generic entity bookkeeping lives in World.cpp.
	class AZUL_ENGINE_LIBRARY_API World
	{
	public:
		static const unsigned int MAX_COMPONENT_TYPES = 64;

		World();
		World(const World &) = delete;
		World &operator=(const World &) = delete;
		~World();

		// Entity lifetime (non-generic -> World.cpp, exported)
		Entity Create();
		void Destroy(const Entity &e);
		bool IsAlive(const Entity &e) const;

		template <typename C>
		C &Add(const Entity &e)
		{
			return this->Pool<C>().Add(e);
		}

		template <typename C>
		C *TryGet(const Entity &e)
		{
			return this->Pool<C>().TryGet(e);
		}

		template <typename C>
		bool Has(const Entity &e)
		{
			return this->Pool<C>().Has(e);
		}

		// Direct pool access for systems that iterate a component's dense array.
		template <typename C>
		ComponentPool<C> &Pool()
		{
			const unsigned int id = C::kTypeId;
			assert(id < MAX_COMPONENT_TYPES);
			if (poPools[id] == nullptr)
			{
				poPools[id] = new ComponentPool<C>();
			}
			return *static_cast<ComponentPool<C> *>(poPools[id]);
		}

	private:
		void privGrowGenerations();
		void privPushFree(unsigned int slot);

		// entity slots
		unsigned int *poGeneration;   // generation per slot (owned)
		unsigned int mSlotCap;
		unsigned int mSlotCount;      // high-water: slots ever handed out
		unsigned int *poFreeList;     // recycled slot indices (owned)
		unsigned int mFreeCount;
		unsigned int mFreeCap;

		// component pools, indexed by C::kTypeId (owned)
		IComponentPool *poPools[MAX_COMPONENT_TYPES];
	};
}

#endif

// --- End of File ---
