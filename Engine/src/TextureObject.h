//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H

#include <d3d11.h>
#include "BufferSamplerState_ps.h"
#include "BufferTextureSRV_ps.h"
#include "BufferTexture2D.h"

namespace Azul
{
	class TextureObject
	{
	public:
		enum class Name
		{
			NullTexture,
			Duckweed,
			Brick,
			Stone,
			Rocks,
			Crate,
			SpaceFrigate,
			ChickenBot,
			R2D2_BASE,
			DogBot,
			Mousey,
			SpiderBot,
			Test0,
			Test1,
			Test2,
			Test3,
			Stitch,
			FontAriel36,

			Uninitialized
		};

	public:
		TextureObject() = default;
		TextureObject(const TextureObject &) = delete;
		TextureObject &operator = (const TextureObject &) = delete;
		virtual ~TextureObject();

		TextureObject(const char *const pMeshFileName,
					  D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

		void ActivateTexture();

		bool HasAlpha() const;

		char *GetName();
		void SetName(TextureObject::Name _name);

	public:


		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		TextureObject::Name mName;

		BufferTextureSRV_ps       SRVTexBuffer;
		BufferSamplerState_ps SamplerStateBuffer;
		BufferTexture2D    Texture2DBuffer;

		unsigned int    width;
		unsigned int    height;
		bool AlphaEnabled;
	};
}

#endif

// --- End of File ---
