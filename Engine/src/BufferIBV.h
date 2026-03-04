//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_IBV_H
#define BUFFER_IBV_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferIBV
	{
	public:
		BufferIBV();
		BufferIBV(const BufferIBV &r) = delete;
		BufferIBV &operator=(const BufferIBV &) = delete;
		virtual ~BufferIBV();

		void Initialize(uint32_t numBytes, void *pData);
		//BufferIBV(uint32_t numBytes, void *pData);

		void SetActive();

	private:
		void privCreate(void *pData);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t     numBytes;
		ID3D11Buffer *poBufferIBV;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
