//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "FontEntry.h"

namespace Azul
{
	FontEntry::~FontEntry()
	{

	}

	FontEntry::FontEntry()
		: key(0),
		x(-1.0),
		y(-1.0),
		width(-1.0),
		height(-1.0f)
	{

	}

	FontEntry::FontEntry(unsigned int _key, float _x, float _y, float _width, float _height)
		: key(_key),
		x(_x),
		y(_y),
		width(_width),
		height(_height)
	{

	}

	void FontEntry::set(unsigned int _key, float _x, float _y, float _width, float _height)
	{
		this->key = _key;
		this->x = _x;
		this->y = _y;
		this->width = _width;
		this->height = _height;
	}


	void FontEntry::Serialize(FontEntry_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		out.set_key(this->key);
		out.set_x(this->x);
		out.set_y(this->y);
		out.set_width(this->width);
		out.set_height(this->height);
	}

	void FontEntry::Deserialize(const FontEntry_proto& in)
	{
		AZUL_UNUSED_VAR(in);

		this->key = in.key();
		this->x = in.x();
		this->y = in.y();
		this->width = in.width();
		this->height = in.height();

	}

	void FontEntry::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);

		Trace::out("key:%3d (x,y,w,h) %f %f %f %f \n",
			key, x, y, width, height);
	}
}

// --- End of File ---


