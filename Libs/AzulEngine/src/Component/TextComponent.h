#ifndef AZUL_TEXT_COMPONENT_H
#define AZUL_TEXT_COMPONENT_H

#include "Glyph.h"
#include "ComponentId.h"

namespace Azul
{
	// A screen-space text string (created via Text2D). The SpriteRenderSystem's
	// text pass reads this: it walks pMessage character by character, looks each
	// glyph up in GlyphMan, and re-fills the entity's Sprite2DComponent (texture +
	// UV + screen-rect) per glyph to draw the run.
	//
	// pMessage is NON-owning and is not copied -- the caller (a scene) must pass a
	// string with stable lifetime (a string literal). The color/mesh/shader live
	// in the same entity's Sprite2DComponent.
	struct TextComponent
	{
		const char *pMessage;    // non-owning; caller keeps the string alive
		Glyph::Name glyphName;
		float x;                 // lower-left start position (screen space)
		float y;

		static const unsigned int kTypeId = COMPONENT_TEXT;
	};
}

#endif

// --- End of File ---
