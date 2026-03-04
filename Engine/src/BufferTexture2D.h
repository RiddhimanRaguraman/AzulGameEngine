//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_TEXTURE_2D_H
#define BUFFER_TEXTURE_2D_H

#include <d3d11.h>
#include "ShaderMappings.h"
#include "textureData.h"

namespace Azul
{
	class BufferTexture2D
	{
	public:
		BufferTexture2D();
		BufferTexture2D(ID3D11Texture2D *p);
		BufferTexture2D(const BufferTexture2D &r) = delete;
		BufferTexture2D &operator=(const BufferTexture2D &) = delete;
		virtual ~BufferTexture2D();

		void Initialize(unsigned int Width,
						unsigned int Height,
						textureData::TEXTURE_NCOMPONENT component,
						unsigned char *pData);

		ID3D11Texture2D *GetID3D11Texture2D();
		bool GetAlphaMode();

	private:
		void privCreate();
		void privInitialize(ID3D11Texture2D *p);

		// ----------------------
		//  Data
		// ----------------------
	public:
		unsigned int mWidth;
		unsigned int mHeight;
		DXGI_FORMAT  mFormat;
		unsigned char *pData;
		bool bAlphaEnabled;

		ID3D11Texture2D *poTex;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
