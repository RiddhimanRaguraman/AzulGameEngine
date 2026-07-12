//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_RENDER_SYSTEM_H
#define AZUL_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"
#include "MathEngine.h"

namespace Azul
{
	class World;
	struct RenderComponent;

	// Draw-phase render dispatch. Owns the per-MaterialKind render logic (the
	// migrated GraphicsObject_*::SetState/SetDataGPU/Draw/RestoreState bodies).
	//
	// P4.2 ORDERING NOTE: this is driven per-object from GameObject::Draw, so 3D
	// objects render in the authoritative **PCS-tree order** (the PCS tree inserts
	// children at the front, so its DFS order is NOT the component-pool insertion
	// order). Alpha-blended objects (e.g. the skybox drawn as a backdrop) depend on
	// that order, so pool-order iteration produced wrong blending. Batched
	// pool iteration + material sorting is a Phase 6 concern once ordering moves
	// fully into the ECS (Phase 5).
	class AZUL_ENGINE_LIBRARY_API RenderSystem
	{
	public:
		// P5.0: draw all 3D renderables from the RenderComponent pool, sorted by
		// `layer` ascending (lower = drawn first). Replaces the per-object 3D draw
		// that the PCS-tree walk used to drive -- the sort provides the draw order
		// the tree used to (e.g. the alpha skybox before the terrain).
		static void Draw(World &world);

	private:
		// Render one 3D renderable via its MaterialKind branch.
		static void DrawObject(RenderComponent &r, Mat4 &world);

		static void privDrawColorByVertex(RenderComponent &r, Mat4 &world);
		static void privDrawFlatTexture(RenderComponent &r, Mat4 &world);
		static void privDrawConstColorLight(RenderComponent &r, Mat4 &world);
		static void privDrawWireframe(RenderComponent &r, Mat4 &world);
		static void privDrawSkinLightTexture(RenderComponent &r, Mat4 &world);
	};
}

#endif

// --- End of File ---
