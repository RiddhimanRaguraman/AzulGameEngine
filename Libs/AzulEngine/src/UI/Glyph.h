//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLYPH_H
#define GLYPH_H

#include "Rect.h"
#include "TextureObject.h"
#include "DLink.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API Glyph : public DLink
	{
	public:

		enum class Name 
		{
			Arial36pt,
			NullObject,
			Uninitialized
		};

	public:
		Glyph();
		Glyph(const Glyph &) = default;
		Glyph &operator = (const Glyph &) = default;
		virtual ~Glyph() override;

		void Set(Name glyphName,int key, TextureObject::Name texName, Rect &glyphRect);


		virtual void Dump() override;
		virtual void Wash() override;

	public:
		// data: -----------------------------------------------------------------------
		Name			glyphName;
		int				key;		
		TextureObject	*pText;
		Rect			glyphRect;

	};

}

#endif

//--- End of File ---

