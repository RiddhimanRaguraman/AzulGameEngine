//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "FontSprite.h"
#include "GlyphMan.h"

namespace Azul
{
	FontSprite::~FontSprite()
	{
		delete[] this->poMessage;
	}

	FontSprite::FontSprite(Mesh::Name mesh, ShaderObject::Name shader, Image::Name image, Rect rect)
		: GameObjectSprite(mesh, shader, image, rect),
		name(FontSprite::Name::Uninitialized),
		pScreenRect(nullptr),
		poMessage(nullptr),
		glyphName(Glyph::Name::Uninitialized),
		x(0.0f),
		y(0.0f)
	{
	}

	void FontSprite::Set(FontSprite::Name _name,
						 const char *_pMessage,
						 Glyph::Name _glyphName,
						 float xStart,
						 float yStart,
						 Color &_color)
	{
		// Deep copy
		assert(_pMessage);
		delete[] this->poMessage;
		size_t size = strlen(_pMessage) + 1;
		this->poMessage = new char[size]();
		memset(this->poMessage, 0, size);
		memcpy_s(this->poMessage, size, _pMessage, size - 1);

		this->x = xStart;
		this->y = yStart;

		this->name = _name;
		this->glyphName = _glyphName;

		this->SetColor(_color);
	}

	void FontSprite::UpdateMessage(const char *_pMessage)
	{
		delete[] this->poMessage;
		// Deep copy
		assert(_pMessage);
		size_t size = strlen(_pMessage) + 1;
		this->poMessage = new char[size]();
		memset(this->poMessage, 0, size);
		memcpy_s(this->poMessage, size, _pMessage, size - 1);
	}

	void FontSprite::Draw()
	{
		assert(poMessage);
		size_t len = strlen(poMessage);

		Rect screenRect;

		float xTmp = this->x;
		float yTmp = this->y;

		float xEnd = this->x;

		for(size_t i = 0; i < len; i++)
		{
			int key = (int)poMessage[i];

			Glyph *pGlyph = GlyphMan::Find(this->glyphName, key);
			assert(pGlyph);

			xTmp = xEnd + pGlyph->glyphRect.width / 2;
			screenRect.Set(xTmp, yTmp, pGlyph->glyphRect.width, pGlyph->glyphRect.height);

			// Configure the per-glyph 2D-sprite state (data-path -> Sprite2DComponent).
			this->SetTexture(pGlyph->pText);
			this->SetImage(pGlyph->glyphRect, pGlyph->pText);
			this->SetScreenRect(screenRect);

			this->posX = xTmp;
			this->posY = yTmp;
			GameObjectSprite::Update(AnimTime());   // world = S*R*T for this glyph
			GameObjectSprite::Draw();               // SpriteRenderSystem draws it

			// move the starting to the next character
			xEnd = pGlyph->glyphRect.width / 2 + xTmp;
		}
	}


	void FontSprite::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

	}

	void FontSprite::Update(AnimTime currentTime)
	{
		// Data-path: nothing to do here -- each glyph's world is computed in Draw
		// (per-glyph GameObjectSprite::Update).
		AZUL_UNUSED_VAR(currentTime);
	}

}

//--- End of File ---
