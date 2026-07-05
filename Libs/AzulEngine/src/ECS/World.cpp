//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "World.h"

namespace Azul
{
	World::World()
		: poGeneration(nullptr),
		mSlotCap(0),
		mSlotCount(0),
		poFreeList(nullptr),
		mFreeCount(0),
		mFreeCap(0)
	{
		for (unsigned int i = 0; i < MAX_COMPONENT_TYPES; i++)
		{
			poPools[i] = nullptr;
		}
	}

	World::~World()
	{
		// Each pool's virtual destructor frees its own arrays.
		for (unsigned int i = 0; i < MAX_COMPONENT_TYPES; i++)
		{
			delete poPools[i];
			poPools[i] = nullptr;
		}
		delete[] poGeneration;
		poGeneration = nullptr;
		delete[] poFreeList;
		poFreeList = nullptr;
	}

	Entity World::Create()
	{
		Entity e;

		if (mFreeCount > 0)
		{
			// recycle a slot; its generation was already bumped on Destroy
			unsigned int slot = poFreeList[mFreeCount - 1];
			mFreeCount--;
			e.index = slot;
			e.generation = poGeneration[slot];
		}
		else
		{
			if (mSlotCount >= mSlotCap)
			{
				this->privGrowGenerations();
			}
			e.index = mSlotCount;
			e.generation = 0;
			poGeneration[mSlotCount] = 0;
			mSlotCount++;
		}
		return e;
	}

	bool World::IsAlive(const Entity &e) const
	{
		if (e.index >= mSlotCount)
		{
			return false;
		}
		return poGeneration[e.index] == e.generation;
	}

	void World::Destroy(const Entity &e)
	{
		if (!this->IsAlive(e))
		{
			return;
		}

		// drop this entity's component from every pool
		for (unsigned int i = 0; i < MAX_COMPONENT_TYPES; i++)
		{
			if (poPools[i] != nullptr)
			{
				poPools[i]->OnEntityDestroyed(e);
			}
		}

		// bump generation so old handles fail IsAlive, then recycle the slot
		poGeneration[e.index]++;
		this->privPushFree(e.index);
	}

	void World::privGrowGenerations()
	{
		unsigned int newCap = (mSlotCap == 0) ? 64 : mSlotCap * 2;
		unsigned int *n = new unsigned int[newCap];
		for (unsigned int i = 0; i < mSlotCount; i++)
		{
			n[i] = poGeneration[i];
		}
		for (unsigned int i = mSlotCount; i < newCap; i++)
		{
			n[i] = 0;
		}
		delete[] poGeneration;
		poGeneration = n;
		mSlotCap = newCap;
	}

	void World::privPushFree(unsigned int slot)
	{
		if (mFreeCount >= mFreeCap)
		{
			unsigned int newCap = (mFreeCap == 0) ? 64 : mFreeCap * 2;
			unsigned int *n = new unsigned int[newCap];
			for (unsigned int i = 0; i < mFreeCount; i++)
			{
				n[i] = poFreeList[i];
			}
			delete[] poFreeList;
			poFreeList = n;
			mFreeCap = newCap;
		}
		poFreeList[mFreeCount] = slot;
		mFreeCount++;
	}
}

// --- End of File ---
