//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_IBV_H
#define BUFFER_IBV_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferIBV_ia
	{
	public:
		BufferIBV_ia();
		BufferIBV_ia(const BufferIBV_ia &r) = delete;
		BufferIBV_ia &operator=(const BufferIBV_ia &) = delete;
		virtual ~BufferIBV_ia();

		void Initialize(uint32_t numBytes, void *pData);
		//BufferIBV_ia(uint32_t numBytes, void *pData);

		void SetActive();

	private:
		void privCreate(void *pData);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t     numBytes;
		ID3D11Buffer *poBufferIBV_ia;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
