//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "RenderSystem.h"
#include "World.h"
#include "RenderComponent.h"
#include "GraphicsObject.h"

namespace Azul
{
	void RenderSystem::Draw(World &world)
	{
		ComponentPool<RenderComponent> &pool = world.Pool<RenderComponent>();
		const unsigned int count = pool.GetCount();

		for (unsigned int i = 0; i < count; i++)
		{
			RenderComponent &r = pool.GetData(i);

			// 3D materials only; 2D/UI (Sprite) stays on the tree walk for now.
			if (!r.drawEnable || !MaterialKindIs3D(r.kind))
			{
				continue;
			}

			assert(r.pGraphicsObject);
			r.pGraphicsObject->Render();
		}
	}
}

// --- End of File ---
