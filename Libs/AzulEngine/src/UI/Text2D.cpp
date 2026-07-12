//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Text2D.h"
#include "WorldMan.h"
#include "World.h"
#include "Sprite2DComponent.h"
#include "TextComponent.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "ImageMan.h"
#include "TextureObject.h"

namespace Azul
{
	Entity Text2D::Add(Mesh::Name mesh,
					   ShaderObject::Name shader,
					   Image::Name imageName,
					   const char *pMessage,
					   Glyph::Name glyph,
					   float x,
					   float y,
					   const Color &color)
	{
		assert(pMessage);

		World &world = WorldMan::GetWorld();
		Entity e = world.Create();

		// 2D-sprite render state (was GraphicsObject_Sprite / GameObjectSprite ctor).
		// texture/uvMatrix/origMatrix are per-glyph scratch that the text pass
		// overwrites; seed them from the image so the component is well-formed.
		Sprite2DComponent &s = world.Add<Sprite2DComponent>(e);
		s.pMesh = MeshNodeMan::Find(mesh);
		s.pShader = ShaderObjectNodeMan::Find(shader);
		assert(s.pMesh);
		assert(s.pShader);

		Image *pImage = ImageMan::Find(imageName);
		assert(pImage);
		s.pTexture = pImage->pText;

		float w = pImage->imageRect.width / (float)pImage->pText->width;
		float h = pImage->imageRect.height / (float)pImage->pText->height;
		float u = pImage->imageRect.x / (float)pImage->pText->width;
		float v = pImage->imageRect.y / (float)pImage->pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		s.uvMatrix = ScaleUV * TransUV;

		s.origMatrix = Scale(1.0f, 1.0f, 1.0f);

		s.color.Set(color);

		// The text run itself (was FontSprite's message/glyph/x/y).
		TextComponent &t = world.Add<TextComponent>(e);
		t.pMessage = pMessage;   // non-owning; caller keeps the buffer alive
		t.glyphName = glyph;
		t.x = x;
		t.y = y;

		return e;
	}

	void Text2D::SetMessage(const Entity &e, const char *pMessage)
	{
		assert(pMessage);
		TextComponent *pT = WorldMan::GetWorld().TryGet<TextComponent>(e);
		assert(pT);
		pT->pMessage = pMessage;   // non-owning; caller keeps the buffer alive
	}

	void Text2D::SetColor(const Entity &e, const Color &color)
	{
		Sprite2DComponent *pS = WorldMan::GetWorld().TryGet<Sprite2DComponent>(e);
		assert(pS);
		pS->color.Set(color);
	}
}

// --- End of File ---
