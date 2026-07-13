#ifndef AZUL_TRANSFORM_COMPONENT_H
#define AZUL_TRANSFORM_COMPONENT_H

#include "MathEngine.h"
#include "ComponentId.h"

namespace Azul
{
	// Per-entity transform. Plain data, no logic.
	// Phase 2 bridge: for now only `world` is wired (it backs
	// GameObject::GetWorld/SetWorld). pos/rot/scale become live when the
	// GameObjectRigidBody trans/quat/scale members migrate here.
	struct TransformComponent
	{
		Vec3 pos;
		Quat rot;
		Vec3 scale;
		Mat4 world;

		static const unsigned int kTypeId = COMPONENT_TRANSFORM;
	};
}

#endif

// --- End of File ---
