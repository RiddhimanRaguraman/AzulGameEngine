#ifndef AZUL_SPRITE2D_COMPONENT_H
#define AZUL_SPRITE2D_COMPONENT_H

#include "MathEngine.h"
#include "Color.h"
#include "ComponentId.h"

namespace Azul
{
	class Mesh;
	class ShaderObject;
	class TextureObject;

	// The mutable 2D-sprite render state. For a plain sprite it's set once; for a
	// text run (Text2D) it's re-filled per glyph (texture + uvMatrix + origMatrix)
	// by SpriteRenderSystem's text pass. The SpriteRenderSystem reads it + a world
	// matrix to draw one 2D quad.
	struct Sprite2DComponent
	{
		Mesh *pMesh;
		ShaderObject *pShader;
		TextureObject *pTexture;
		Mat4 uvMatrix;     // sub-image UV transform
		Mat4 origMatrix;   // screen-rect scale
		Color color;       // color scale

		static const unsigned int kTypeId = COMPONENT_SPRITE2D;
	};
}

#endif

// --- End of File ---
