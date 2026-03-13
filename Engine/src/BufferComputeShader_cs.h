//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_COMPUTE_SHADER_CS_H
#define BUFFER_COMPUTE_SHADER_CS_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferComputeShader_cs
	{
	public:
		BufferComputeShader_cs() = delete;
		BufferComputeShader_cs(const BufferComputeShader_cs &r) = delete;
		BufferComputeShader_cs &operator=(const BufferComputeShader_cs &) = delete;
		virtual ~BufferComputeShader_cs();

		BufferComputeShader_cs(size_t numBytes, void *pData);

		void SetActive();

	private:
		void privCreate(void *pData);

		// ----------------------
		//  Data
		// ----------------------
	public:
		size_t		         codeSize;
		ID3D11ComputeShader *poComputeShader;
		bool                 bCreate;
	};
}

#endif

//--- End of File ---
