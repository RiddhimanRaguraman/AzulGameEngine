//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_RENDER_SYSTEM_H
#define AZUL_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"

namespace Azul
{
	class World;

	// Draw-phase system (NOT a SystemMan/Update system). Iterates the
	// RenderComponent pool and draws the 3D materials (FlatTexture,
	// SkinLightTexture) in one flat pass -- replacing the 3D portion of the
	// GameObjectMan::Draw PCS-tree walk. The 2D/UI (Sprite + fonts) are handled
	// separately (P4.3 SpriteRenderSystem); for now they stay on the tree walk.
	//
	// P4.1 bridge: still calls the existing GraphicsObject->Render() per object;
	// P4.2 will move the SetState/SetDataGPU/Draw/RestoreState bodies into
	// per-MaterialKind branches here and batch by shader/material.
	class AZUL_ENGINE_LIBRARY_API RenderSystem
	{
	public:
		RenderSystem() = default;
		RenderSystem(const RenderSystem &) = delete;
		RenderSystem &operator=(const RenderSystem &) = delete;
		~RenderSystem() = default;

		void Draw(World &world);
	};
}

#endif

// --- End of File ---
