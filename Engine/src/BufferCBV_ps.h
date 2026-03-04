//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_CBV_PS_H
#define BUFFER_CBV_PS_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferCBV_ps
	{
	public:
		BufferCBV_ps() = delete;
		BufferCBV_ps(const BufferCBV_ps &r) = delete;
		BufferCBV_ps &operator=(const BufferCBV_ps &) = delete;
		virtual ~BufferCBV_ps();

		BufferCBV_ps(uint32_t structSize);

		void Transfer(void *pBuff);
		void SetActive(ConstantPSBufferSlot slot);

	private:
		void privCreate(uint32_t structSize);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t     structSize;
		ID3D11Buffer *poBufferCBV;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
