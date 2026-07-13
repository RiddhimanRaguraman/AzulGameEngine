#ifndef AZUL_RENDERABLE3D_H
#define AZUL_RENDERABLE3D_H

#include "MaterialKind.h"
#include "Entity.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	struct TransformComponent;
	struct RenderComponent;

	// Factory + accessors for a 3D renderable (replaces GameObjectRigidBody /
	// GameObjectTerrain).
	//
	// A renderable is a PURE ECS entity -- NOT a GameObject / PCS node: a
	// TransformComponent (placement) + a RenderComponent (the MaterialKind plus the
	// mesh/shader/tex/light/... handles the caller fills via GetRender). The
	// LocalToWorld / Render (+ optional Rotate) systems drive it. No per-object
	// Update/Draw and no new/delete: the entity + components live in the World and
	// are freed by WorldMan::Destroy at scene unload.
	class AZUL_ENGINE_LIBRARY_API Renderable3D
	{
	public:
		// Create the entity with a seeded identity transform and a RenderComponent
		// of the given kind (all handles null). Fill the handles via GetRender().
		static Entity Add(MaterialKind kind);

		static TransformComponent &GetTransform(const Entity &e);
		static RenderComponent &GetRender(const Entity &e);

		// Placement helpers (match the old GameObjectRigidBody setters).
		static void SetTrans(const Entity &e, float x, float y, float z);
		static void SetScale(const Entity &e, float s);
		static void SetScale(const Entity &e, float sx, float sy, float sz);
	};
}

#endif

// --- End of File ---
