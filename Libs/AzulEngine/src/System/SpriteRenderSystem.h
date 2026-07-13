#ifndef AZUL_SPRITE_RENDER_SYSTEM_H
#define AZUL_SPRITE_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"
#include "MathEngine.h"

namespace Azul
{
	class World;
	class ShaderObject;
	struct Sprite2DComponent;
	struct TextComponent;

	// 2D/UI render dispatch: orthographic camera, screen-rect + world + lower-left
	// origin transform, sub-image UV, and color scale, with alpha blending.
	//
	// Draw() is the pool pass that draws all 2D/UI from the component pools, after
	// the opaque 3D pass (RenderSystem::Draw). Plain sprites draw one quad from
	// their Sprite2DComponent + world; text entities (TextComponent) run the
	// per-glyph loop.
	class AZUL_ENGINE_LIBRARY_API SpriteRenderSystem
	{
	public:
		static void Draw(World &world);

	private:
		// Draw one 2D quad. pLastShader hoists the shader/CBV bind across a
		// same-shader run.
		static void DrawSprite(Sprite2DComponent &s, Mat4 &world, ShaderObject *&pLastShader);

		// Draw one text run: walk t.pMessage, re-filling `s` per glyph.
		static void DrawText(TextComponent &t, Sprite2DComponent &s, ShaderObject *&pLastShader);
	};
}

#endif

// --- End of File ---
