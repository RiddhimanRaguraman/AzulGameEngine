//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferUAV_cs::BufferUAV_cs(size_t _count, size_t _structSize)
		: count{ _count },
		structSize{ _structSize },
		pBuff{ nullptr },
		poComputeUAVBuffer{ nullptr },
		poUnorderedAccessView{ nullptr },
		bCreate{ false }
	{
		this->privCreate(_count, _structSize);
	}

	void BufferUAV_cs::privCreate(size_t _count, size_t _structSize)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		this->count = _count;
		this->structSize = _structSize;
		assert(count > 0);
		assert(structSize > 0);

		HRESULT hr;

		// --------------------------
		// Create the buffer
		// --------------------------

		D3D11_BUFFER_DESC BufferDesc{ 0 };

		BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		BufferDesc.ByteWidth = (unsigned int)(this->count * this->structSize);
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		BufferDesc.StructureByteStride = (unsigned int)this->structSize;

		hr = StateDirectXMan::GetDevice()->CreateBuffer(&BufferDesc,
			nullptr,
			&this->poComputeUAVBuffer);
		assert(SUCCEEDED(hr));


		// --------------------------
		// Create the UAV buffer
		// --------------------------

		D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;

		sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

		sbUAVDesc.Buffer.FirstElement = 0;
		sbUAVDesc.Buffer.Flags = 0;
		sbUAVDesc.Buffer.NumElements = (unsigned int)this->count;  // number of structures

		hr = StateDirectXMan::GetDevice()->CreateUnorderedAccessView(this->poComputeUAVBuffer,
			&sbUAVDesc,
			&this->poUnorderedAccessView);
		assert(SUCCEEDED(hr));
	}

	void BufferUAV_cs::Transfer(void* _pBuff)
	{
		// in case set was called before
		assert(this->bCreate == true);

		this->pBuff = _pBuff;
		assert(pBuff);

		StateDirectXMan::GetContext()->UpdateSubresource(poComputeUAVBuffer,
			0,
			nullptr,
			pBuff,
			0,
			0);
	}

	void BufferUAV_cs::BindCompute(UnorderedAccessBufferSlot slot)
	{
		StateDirectXMan::GetContext()->CSSetUnorderedAccessViews((size_t)slot,
			1,
			&poUnorderedAccessView,
			nullptr);

	}

	ID3D11Buffer *BufferUAV_cs::GetD3DBuffer()
	{
		return this->poComputeUAVBuffer;
	}


	BufferUAV_cs::~BufferUAV_cs()
	{
		SafeRelease(this->poComputeUAVBuffer);
		SafeRelease(this->poUnorderedAccessView);
	}
}

//--- End of File ---