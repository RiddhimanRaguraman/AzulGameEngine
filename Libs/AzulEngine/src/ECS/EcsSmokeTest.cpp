//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include "EcsSmokeTest.h"
#include "World.h"

namespace Azul
{
	// Two throwaway components with explicit type ids (the id convention every
	// real component will follow).
	struct TestPosition
	{
		float x;
		float y;
		float z;
		static const unsigned int kTypeId = 0;
	};

	struct TestVelocity
	{
		float dx;
		float dy;
		float dz;
		static const unsigned int kTypeId = 1;
	};

	void ECS_SmokeTest()
	{
		World world;

		Entity e0 = world.Create();
		Entity e1 = world.Create();
		assert(world.IsAlive(e0));
		assert(world.IsAlive(e1));

		// add components
		TestPosition &p0 = world.Add<TestPosition>(e0);
		p0.x = 1.0f;
		p0.y = 2.0f;
		p0.z = 3.0f;
		world.Add<TestVelocity>(e0);
		world.Add<TestPosition>(e1);

		assert(world.Has<TestPosition>(e0));
		assert(world.Has<TestVelocity>(e0));
		assert(world.Has<TestPosition>(e1));
		assert(!world.Has<TestVelocity>(e1));

		// read back
		TestPosition *pGet = world.TryGet<TestPosition>(e0);
		assert(pGet != nullptr);
		assert(pGet->x == 1.0f && pGet->y == 2.0f && pGet->z == 3.0f);

		// iterate the dense array the way a system will
		ComponentPool<TestPosition> &posPool = world.Pool<TestPosition>();
		assert(posPool.GetCount() == 2);
		unsigned int seen = 0;
		for (unsigned int i = 0; i < posPool.GetCount(); i++)
		{
			const Entity &owner = posPool.GetOwner(i);
			assert(world.IsAlive(owner));
			seen++;
		}
		assert(seen == 2);

		// destroy e0: its components are dropped, its handle goes stale
		world.Destroy(e0);
		assert(!world.IsAlive(e0));
		assert(!world.Has<TestPosition>(e0));
		assert(!world.Has<TestVelocity>(e0));
		assert(world.Has<TestPosition>(e1));
		assert(world.Pool<TestPosition>().GetCount() == 1);

		// recycle: the freed slot comes back with a new generation
		Entity e2 = world.Create();
		assert(e2.index == e0.index);
		assert(e2.generation != e0.generation);
		assert(world.IsAlive(e2));
		assert(!world.IsAlive(e0));   // old handle still stale

		Trace::out("ECS smoke test PASSED\n");
	}
}

// --- End of File ---
