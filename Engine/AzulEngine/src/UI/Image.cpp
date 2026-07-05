//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TexNodeMan.h"
#include "Image.h"
#include "Rect.h"
#include "StringThis.h"

namespace Azul
{
	// public methods: -------------------------------------------------------------

	Image::Image()
	{
		this->imageName = Image::Name::Not_Initialized;
		this->imageRect.clear();
		this->pText = nullptr;
	}

	Image::~Image()
	{
		//Trace::out("Image::~Image() %p\n", this);
	}

	Image::Image(Image::Name _imageName, TextureObject *_pText, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}

	Image::Image(Image::Name _imageName, TextureObject::Name _textName, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = TexNodeMan::Find(_textName);
		assert(pText);
		this->imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, TextureObject *_pText, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = _pText;
		this->imageRect = _rect;
	}

	void Image::Set(Image::Name _imageName, TextureObject::Name _textName, Rect _rect)
	{
		this->imageName = _imageName;
		this->pText = TexNodeMan::Find(_textName);
		assert(pText);
		this->imageRect = _rect;
	}




	void Image::Dump()
	{
		Trace::out("      Image(%p)\n", this);

		// Data:
		//Trace::out("      Name: %s \n", StringMe(this->name));
		//Trace::out("         x: %d \n", this->x);

		DLink::Dump();
	}

	void Image::Wash()
	{
		this->imageName = Image::Name::Not_Initialized;
		this->imageRect.clear();
		this->pText = nullptr;
	}
}

//--- End of File ---
