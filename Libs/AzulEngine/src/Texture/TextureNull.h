//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_NULL_H
#define TEXTURE_NULL_H
#include "EngineDLLInterface.h"

#include "TextureObject.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API TextureNull : public TextureObject
	{
	public:
		TextureNull();
		TextureNull(const TextureNull &) = delete;
		TextureNull &operator=(const TextureNull &other) = delete;
		virtual ~TextureNull();


	private:

	};
}

#endif

// --- End of File ---
