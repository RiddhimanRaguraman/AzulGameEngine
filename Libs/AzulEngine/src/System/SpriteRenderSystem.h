//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_SPRITE_RENDER_SYSTEM_H
#define AZUL_SPRITE_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"
#include "MathEngine.h"

namespace Azul
{
	struct Sprite2DComponent;

	// 2D/UI render dispatch. Owns the sprite render logic (was
	// GraphicsObject_Sprite::SetState/SetDataGPU/Draw/RestoreState): orthographic
	// camera, screen-rect + world + lower-left origin transform, sub-image UV, and
	// color scale, with alpha blending.
	//
	// Driven per-object from the 2D draw path (GameObjectSprite::Draw, and
	// FontSprite::Draw's per-glyph loop) so 2D keeps PCS-tree order and draws after
	// the opaque 3D pass.
	class AZUL_ENGINE_LIBRARY_API SpriteRenderSystem
	{
	public:
		static void DrawSprite(Sprite2DComponent &s, Mat4 &world);
	};
}

#endif

// --- End of File ---
