//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "RotateSystem.h"
#include "World.h"
#include "TransformComponent.h"
#include "RotateComponent.h"

namespace Azul
{
	RotateSystem::~RotateSystem()
	{
	}

	void RotateSystem::Update(World &world, AnimTime tCurr)
	{
		AZUL_UNUSED_VAR(tCurr);

		ComponentPool<RotateComponent> &pool = world.Pool<RotateComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			RotateComponent &rc = pool.GetData(i);
			const Entity &e = pool.GetOwner(i);

			TransformComponent *pT = world.TryGet<TransformComponent>(e);
			if (pT == nullptr)
			{
				continue;
			}

			rc.angle += rc.speed;

			Scale S(pT->scale.x(), pT->scale.y(), pT->scale.z());
			Rot   baseRot(pT->rot);
			Rot   spin(Rot1::Z, rc.angle);
			Trans T(pT->pos.x(), pT->pos.y(), pT->pos.z());

			pT->world = S * baseRot * spin * T;
		}
	}
}

// --- End of File ---
