//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ECS_COMPONENT_POOL_H
#define AZUL_ECS_COMPONENT_POOL_H

#include <assert.h>
#include "Entity.h"
#include "IComponentPool.h"

namespace Azul
{
	// Sparse-set storage for one component type C.
	//
	// TEMPLATE EXCEPTION (WhatToDo.md 0a): component storage is inherently
	// generic over C, and templates are not dll-exported, so this is header-
	// only inline. No STL: three hand-managed arrays grown by realloc-and-copy.
	//   sparse[entity.index] -> dense slot, or ECS_INVALID_INDEX
	//   denseData[slot]      -> the component
	//   denseOwner[slot]     -> the owning Entity (parallel to denseData)
	// Dense packing keeps live components contiguous for cache-friendly loops.
	template <typename C>
	class ComponentPool : public IComponentPool
	{
	public:
		ComponentPool()
			: sparse(nullptr), sparseCap(0),
			denseData(nullptr), denseOwner(nullptr),
			denseCount(0), denseCap(0)
		{
		}
		ComponentPool(const ComponentPool &) = delete;
		ComponentPool &operator=(const ComponentPool &) = delete;
		virtual ~ComponentPool()
		{
			delete[] sparse;
			delete[] denseData;
			delete[] denseOwner;
		}

		C &Add(const Entity &e)
		{
			privEnsureSparse(e.index);
			assert(sparse[e.index] == ECS_INVALID_INDEX);   // not already present
			privEnsureDense(denseCount + 1);

			unsigned int slot = denseCount;
			sparse[e.index] = slot;
			denseOwner[slot] = e;
			denseCount++;
			return denseData[slot];
		}

		bool Has(const Entity &e) const
		{
			if (e.index >= sparseCap)
			{
				return false;
			}
			unsigned int slot = sparse[e.index];
			if (slot == ECS_INVALID_INDEX)
			{
				return false;
			}
			// stale-handle guard: the stored owner must match this generation
			return denseOwner[slot].generation == e.generation;
		}

		C *TryGet(const Entity &e)
		{
			if (!this->Has(e))
			{
				return nullptr;
			}
			return &denseData[sparse[e.index]];
		}

		void Remove(const Entity &e)
		{
			if (e.index >= sparseCap)
			{
				return;
			}
			unsigned int slot = sparse[e.index];
			if (slot == ECS_INVALID_INDEX)
			{
				return;
			}
			// swap-remove: move the last dense element into the hole
			unsigned int last = denseCount - 1;
			denseData[slot] = denseData[last];
			denseOwner[slot] = denseOwner[last];
			sparse[denseOwner[slot].index] = slot;
			sparse[e.index] = ECS_INVALID_INDEX;
			denseCount--;
		}

		virtual void OnEntityDestroyed(const Entity &e) override
		{
			this->Remove(e);
		}

		// Iteration surface for systems (explicit loops -- no lambdas / views):
		unsigned int GetCount() const { return denseCount; }
		C &GetData(unsigned int slot) { return denseData[slot]; }
		const Entity &GetOwner(unsigned int slot) const { return denseOwner[slot]; }

	private:
		void privEnsureSparse(unsigned int index)
		{
			if (index < sparseCap)
			{
				return;
			}
			unsigned int newCap = (sparseCap == 0) ? 64 : sparseCap;
			while (newCap <= index)
			{
				newCap *= 2;
			}
			unsigned int *n = new unsigned int[newCap];
			for (unsigned int i = 0; i < sparseCap; i++)
			{
				n[i] = sparse[i];
			}
			for (unsigned int i = sparseCap; i < newCap; i++)
			{
				n[i] = ECS_INVALID_INDEX;
			}
			delete[] sparse;
			sparse = n;
			sparseCap = newCap;
		}

		void privEnsureDense(unsigned int need)
		{
			if (need <= denseCap)
			{
				return;
			}
			unsigned int newCap = (denseCap == 0) ? 64 : denseCap;
			while (newCap < need)
			{
				newCap *= 2;
			}
			C *nd = new C[newCap];
			Entity *no = new Entity[newCap];
			for (unsigned int i = 0; i < denseCount; i++)
			{
				nd[i] = denseData[i];
				no[i] = denseOwner[i];
			}
			delete[] denseData;
			delete[] denseOwner;
			denseData = nd;
			denseOwner = no;
			denseCap = newCap;
		}

		// Data (owned; freed in the destructor)
		unsigned int *sparse;
		unsigned int sparseCap;
		C *denseData;
		Entity *denseOwner;
		unsigned int denseCount;
		unsigned int denseCap;
	};
}

#endif

// --- End of File ---
