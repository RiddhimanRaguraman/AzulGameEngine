//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TexNodeMan.h"
#include "Glyph.h"
#include "Rect.h"
#include "StringThis.h"

namespace Azul
{
	// public methods: ------------------------------------------------------------
	Glyph::~Glyph()
	{
		//Trace::out("Image::~Image() %p\n", this);
	}

	Glyph::Glyph()
	{
		this->glyphName = Glyph::Name::Uninitialized;
		this->glyphRect.clear();
		this->pText = nullptr;
		this->key = 0;
	}

	void Glyph::Set(Name _glyphName, int _key, TextureObject::Name texName, Rect &_glyphRect)
	{
		this->glyphName = _glyphName;
		this->glyphRect = _glyphRect;

		TextureObject *_pText = TexNodeMan::Find(texName);
		assert(_pText);

		this->pText = _pText;
		this->key = _key;
	}




	void Glyph::Dump()
	{
		Trace::out("      Glyph(%p)\n", this);

		// Data:
	//	Trace::out("      Name: %s \n", StringMe(this->glyphName));

		if(this->pText)
		{
			Trace::out("      Texture: %s \n", this->pText->GetName());
		}
		else
		{
			Trace::out("      Texture: %s \n", "null");
		}
		Trace::out("      Rect: %f %f %f %f \n",
				   this->glyphRect.x,
				   this->glyphRect.y,
				   this->glyphRect.width,
				   this->glyphRect.height
		);

		DLink::Dump();
	}

	void Glyph::Wash()
	{
		this->glyphName = Glyph::Name::Uninitialized;
		this->glyphRect.clear();
		this->pText = nullptr;
		this->key = 0;
	}

}

//--- End of File ---
