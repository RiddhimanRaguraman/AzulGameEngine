//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEX_NODE_H
#define TEX_NODE_H

#include "DLink.h"
#include "TextureObject.h"

namespace Azul
{

	class TexNode : public DLink
	{
	public:
		TexNode();
		TexNode(const TexNode &) = delete;
		TexNode &operator = (const TexNode &) = delete;
		virtual ~TexNode();

		void Set(TextureObject::Name name, TextureObject *pTextureObject);
		TextureObject *GetTextureObject();

		virtual char *GetName() override;
		virtual void Wash() override;

		virtual void Dump() override;

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		TextureObject *poTextureObject;
	};

}

#endif

// --- End of File ---
