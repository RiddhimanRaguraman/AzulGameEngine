//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_PIXEL_SHADER_H
#define BUFFER_PIXEL_SHADER_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferPixelShader_ps
	{
	public:
		BufferPixelShader_ps() = delete;
		BufferPixelShader_ps(const BufferPixelShader_ps &r) = delete;
		BufferPixelShader_ps &operator=(const BufferPixelShader_ps &) = delete;
		virtual ~BufferPixelShader_ps();

		BufferPixelShader_ps(uint32_t numBytes, void *pData);

		void SetActive();

	private:
		void privCreate(void *pData);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t		  codeSize;
		ID3D11PixelShader *poPixelShader;
		bool              bCreate;
	};
}

#endif

//--- End of File ---
