//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferCBV_ps.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferCBV_ps::BufferCBV_ps(uint32_t _structSize)
		: structSize{_structSize},
		poBufferCBV{nullptr},
		bCreate{false}
	{
		this->privCreate(_structSize);
	}

	void BufferCBV_ps::privCreate(uint32_t _structSize)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		this->structSize = _structSize;
		assert(structSize > 0);

		D3D11_BUFFER_DESC BufferDesc{0};

		//  shader resource
		BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferDesc.ByteWidth = this->structSize;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = 0;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateBuffer(&BufferDesc,
														nullptr,
														&this->poBufferCBV);
		assert(SUCCEEDED(hr));
		assert(this->poBufferCBV);
	}

	void BufferCBV_ps::Transfer(void *pBuff)
	{
		// in case set was called before
		assert(this->bCreate == true);
		assert(pBuff);

		StateDirectXMan::GetContext()->UpdateSubresource(poBufferCBV,
														 0,
														 nullptr,
														 pBuff,
														 0,
														 0);
	}

	void BufferCBV_ps::SetActive(ConstantPSBufferSlot slot)
	{
		// slow down... check this function (VS,CS,PS) types
		StateDirectXMan::GetContext()->PSSetConstantBuffers((uint32_t)slot,
															1,
															&this->poBufferCBV);
	}

	BufferCBV_ps::~BufferCBV_ps()
	{
		SafeRelease(this->poBufferCBV);
	}

}

//--- End of File ---
