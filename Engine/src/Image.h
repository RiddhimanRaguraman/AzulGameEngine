//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef IMAGE_H
#define IMAGE_H

#include "Rect.h"
#include "TextureObject.h"
#include "DLink.h"

namespace Azul
{
	class Image : public DLink
	{
	public:

		enum Name
		{
			WhiteBird,
			YellowBird,
			GreenBird,
			Stitch,
			ALLBirds,
			Not_Initialized,
		};

	public:
		// public methods: -------------------------------------------------------------
		Image();
		Image(Image::Name imageName, TextureObject::Name textName, Rect rect);
		Image(Image::Name imageName, TextureObject *pTexture, Rect rect);
		virtual ~Image();

		void Set(Image::Name imageName, TextureObject::Name textName, Rect rect);
		void Set(Image::Name imageName, TextureObject *pTexture, Rect rect);
		//EnumName getName();


		virtual void Dump() override;
		virtual void Wash() override;

	public:
		// data: -----------------------------------------------------------------------
		Image::Name	    imageName;
		TextureObject *pText;
		Rect			imageRect;
	};

}


#endif

//--- End of File ---
