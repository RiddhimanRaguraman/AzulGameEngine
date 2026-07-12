//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_SPRITE_RENDER_SYSTEM_H
#define AZUL_SPRITE_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"
#include "MathEngine.h"

namespace Azul
{
	class World;
	struct Sprite2DComponent;
	struct TextComponent;

	// 2D/UI render dispatch. Owns the sprite render logic (was
	// GraphicsObject_Sprite::SetState/SetDataGPU/Draw/RestoreState): orthographic
	// camera, screen-rect + world + lower-left origin transform, sub-image UV, and
	// color scale, with alpha blending.
	//
	// P5.1: Draw() is the pool pass that draws all 2D/UI from the component pools,
	// after the opaque 3D pass (RenderSystem::Draw). This replaced the per-object
	// 2D draw the PCS-tree walk used to drive. Plain sprites draw one quad from
	// their Sprite2DComponent + world; text entities (TextComponent, created via
	// Text2D) run the per-glyph loop that FontSprite::Draw used to do imperatively.
	class AZUL_ENGINE_LIBRARY_API SpriteRenderSystem
	{
	public:
		static void Draw(World &world);

	private:
		// Draw one 2D quad (was GraphicsObject_Sprite::SetState/.../RestoreState).
		static void DrawSprite(Sprite2DComponent &s, Mat4 &world);

		// Draw one text run: walk t.pMessage, re-filling `s` per glyph.
		static void DrawText(TextComponent &t, Sprite2DComponent &s);
	};
}

#endif

// --- End of File ---
