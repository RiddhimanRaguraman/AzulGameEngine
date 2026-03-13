//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_CBV_CS_H
#define BUFFER_CBV_CS_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferCBV_cs
	{
	public:
		BufferCBV_cs() = delete;
		BufferCBV_cs(const BufferCBV_cs &r) = delete;
		BufferCBV_cs &operator=(const BufferCBV_cs &) = delete;
		virtual ~BufferCBV_cs();

		BufferCBV_cs(size_t structSize);

		void Transfer(void *pBuff);
		void BindCompute(ConstantCSBufferSlot slot);

	private:
		void privCreate(size_t structSize);

	// ----------------------
	//  Data
	// -----------------------
	public:
		size_t       structSize;
		void *pBuff;
		ID3D11Buffer *poComputeCBVBuffer;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
