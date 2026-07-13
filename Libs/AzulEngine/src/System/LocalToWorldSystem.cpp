#include "MathEngine.h"
#include "LocalToWorldSystem.h"
#include "World.h"
#include "TransformComponent.h"

namespace Azul
{
	LocalToWorldSystem::~LocalToWorldSystem()
	{
	}

	void LocalToWorldSystem::Update(World &world, AnimTime tCurr)
	{
		AZUL_UNUSED_VAR(tCurr);

		ComponentPool<TransformComponent> &pool = world.Pool<TransformComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			TransformComponent &t = pool.GetData(i);

			Scale S(t.scale.x(), t.scale.y(), t.scale.z());
			Rot   R(t.rot);
			Trans T(t.pos.x(), t.pos.y(), t.pos.z());

			t.world = S * R * T;
		}
	}
}

// --- End of File ---
