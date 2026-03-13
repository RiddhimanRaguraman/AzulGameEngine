//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_SHADER_RESOURCE_VIEW_H
#define BUFFER_SHADER_RESOURCE_VIEW_H

#include <d3d11.h>
#include "ShaderMappings.h"
#include "textureData.h"

namespace Azul
{
	class BufferTexture2D;

	class BufferTextureSRV_ps
	{
	public:
		BufferTextureSRV_ps();
		BufferTextureSRV_ps(const BufferTextureSRV_ps &r) = delete;
		BufferTextureSRV_ps &operator=(const BufferTextureSRV_ps &) = delete;
		virtual ~BufferTextureSRV_ps();

		void Initialize(textureData::TEXTURE_NCOMPONENT component, 
						BufferTexture2D &Texture2dBuffer);

		void SetActive(ShaderResourceBufferSlot slot);

	private:
		void privCreate(BufferTexture2D &rTex2D);

		// ----------------------
		//  Data
		// ----------------------
	public:
		DXGI_FORMAT mFormat;
		ID3D11ShaderResourceView *poSRV;
		bool					 bCreate;
	};
}

#endif

//--- End of File ---
