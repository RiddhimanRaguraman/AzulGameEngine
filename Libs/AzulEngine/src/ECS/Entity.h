//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ECS_ENTITY_H
#define AZUL_ECS_ENTITY_H

namespace Azul
{
	// An Entity is just an id: a slot index plus a generation counter. The
	// generation is bumped when a slot is recycled, so a stale handle to a
	// destroyed entity can be detected. No data, no behavior.
	struct Entity
	{
		unsigned int index;
		unsigned int generation;
	};

	// Reserved "no slot" marker used by the sparse arrays.
	static const unsigned int ECS_INVALID_INDEX = 0xFFFFFFFFu;

	// Trivial handle comparison (POD data, kept inline on purpose).
	inline bool EntitySame(const Entity &a, const Entity &b)
	{
		return (a.index == b.index) && (a.generation == b.generation);
	}

	// "No entity" sentinel (e.g. a hierarchy root has a null parent).
	inline Entity EntityNull()
	{
		Entity e;
		e.index = ECS_INVALID_INDEX;
		e.generation = 0u;
		return e;
	}

	inline bool EntityIsNull(const Entity &e)
	{
		return e.index == ECS_INVALID_INDEX;
	}
}

#endif

// --- End of File ---
