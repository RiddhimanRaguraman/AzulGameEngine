//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_SRV_CS_H
#define BUFFER_SRV_CS_H

#include <d3d11.h>

#include "ShaderMappings.h"

namespace Azul
{
	class BufferSRV_cs
	{
	public:
		BufferSRV_cs();
		BufferSRV_cs(const BufferSRV_cs& r) = delete;
		BufferSRV_cs& operator=(const BufferSRV_cs&) = delete;
		virtual ~BufferSRV_cs();

		BufferSRV_cs(size_t count, size_t structSize);

		void Transfer(void* pBuff);
		void BindComputeToCS(ShaderResourceBufferSlot slot);
		void BindComputeToVS(ShaderResourceBufferSlot slot);
		void Initialize(size_t _count, size_t _structSize);

		ID3D11Buffer *GetD3DBuffer();
	private:
		void privCreate(size_t count, size_t structSize);

		// ----------------------
		//  Data
		// -----------------------
	public:
		size_t count;
		size_t structSize;
		void* pBuff;
		ID3D11Buffer *poComputeSRVBuffer;
		ID3D11ShaderResourceView* poShaderResourceView;
		bool bCreate;
	};
}

#endif

//--- End of File ---