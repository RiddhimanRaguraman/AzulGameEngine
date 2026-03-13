//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferSRV_cs.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferSRV_cs::BufferSRV_cs(size_t _count, size_t _structSize)
		: count{ _count },
		structSize{ _structSize },
		pBuff{ nullptr },
		poComputeRVSBuffer{ nullptr },
		poShaderResourceView{ nullptr },
		bCreate{ false }
	{
		this->privCreate(_count, _structSize);
	}

	void BufferSRV_cs::privCreate(size_t _count, size_t _structSize)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		this->count = _count;
		this->structSize = _structSize;

		assert(count > 0);
		assert(structSize > 0);

		D3D11_BUFFER_DESC BufferDesc{ 0 };

		//  shader resource
		BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		BufferDesc.ByteWidth = this->count * this->structSize;
		BufferDesc.CPUAccessFlags = 0;
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		BufferDesc.StructureByteStride = this->structSize;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateBuffer(&BufferDesc,
			nullptr,
			&this->poComputeRVSBuffer);
		assert(SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = this->count;    // number of structures

		hr = StateDirectXMan::GetDevice()->CreateShaderResourceView(poComputeRVSBuffer, &srvDesc, &this->poShaderResourceView);
		assert(SUCCEEDED(hr));
	}

	void BufferSRV_cs::Transfer(void* _pBuff)
	{
		// in case set was called before
		assert(this->bCreate == true);

		this->pBuff = _pBuff;
		assert(pBuff);

		StateDirectXMan::GetContext()->UpdateSubresource(poComputeRVSBuffer,
			0,
			nullptr,
			pBuff,
			0,
			0);
	}

	void BufferSRV_cs::BindCompute(ShaderResourceBufferSlot slot)
	{

		StateDirectXMan::GetContext()->CSSetShaderResources((size_t)slot,
			1,
			&this->poShaderResourceView);

	}

	void BufferSRV_cs::BindVertex(ShaderResourceBufferSlot slot)
	{

		StateDirectXMan::GetContext()->VSSetShaderResources((size_t)slot,
			1,
			&this->poShaderResourceView);

	}

	BufferSRV_cs::~BufferSRV_cs()
	{
		SafeRelease(this->poComputeRVSBuffer);
		SafeRelease(this->poShaderResourceView);
	}

}

//--- End of File ---