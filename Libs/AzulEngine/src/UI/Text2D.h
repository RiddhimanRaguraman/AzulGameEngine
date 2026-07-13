#ifndef AZUL_TEXT2D_H
#define AZUL_TEXT2D_H

#include "Mesh.h"
#include "ShaderObject.h"
#include "Image.h"
#include "Glyph.h"
#include "Color.h"
#include "Entity.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	// Factory + mutators for a screen-space text run (replaces FontSprite).
	//
	// A text run is a PURE ECS entity -- NOT a GameObject / PCS-tree node. It owns
	// a Sprite2DComponent (mesh + shader + per-glyph scratch texture/UV/screen-rect
	// + color) and a TextComponent (message + glyph + start x/y). SpriteRenderSystem's
	// text pass walks the message and draws each glyph. There is no per-object
	// Update/Draw and no new/delete: the entity + its components live in the World
	// and are freed wholesale by WorldMan::Destroy at scene unload.
	//
	// pMessage is stored NON-OWNING and is not copied -- the caller must pass a
	// string with stable lifetime (a string literal is ideal). Scenes hold the
	// returned Entity if they need to mutate the run later (SetMessage/SetColor).
	class AZUL_ENGINE_LIBRARY_API Text2D
	{
	public:
		static Entity Add(Mesh::Name mesh,
						  ShaderObject::Name shader,
						  Image::Name image,
						  const char *pMessage,
						  Glyph::Name glyph,
						  float x,
						  float y,
						  const Color &color);

		static void SetMessage(const Entity &e, const char *pMessage);
		static void SetColor(const Entity &e, const Color &color);
	};
}

#endif

// --- End of File ---
