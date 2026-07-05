//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferIBV_ia.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferIBV_ia::BufferIBV_ia()
		: numBytes { 0 },
		poBufferIBV_ia { nullptr },
		bCreate { false }
	{
	}

	void BufferIBV_ia::Initialize(uint32_t _numBytes, void *pData)
	{
		assert(bCreate == false);
		this->numBytes = _numBytes;
		assert(pData);
		this->privCreate(pData);
	}


	//BufferIBV_ia::BufferIBV_ia(uint32_t _numBytes, void *pData)
	//	: numBytes { _numBytes },
	//	poBufferIBV_ia { nullptr },
	//	bCreate { false }
	//{
	//	assert(pData);
	//	this->privCreate(pData);
	//}

	void BufferIBV_ia::privCreate(void *pData)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		assert(pData);

		// Create and initialize the index buffer.
		D3D11_BUFFER_DESC indexBufferDesc { 0 };
		D3D11_SUBRESOURCE_DATA resourceData { 0 };

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = this->numBytes;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		resourceData.pSysMem = pData;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateBuffer(&indexBufferDesc,
														 &resourceData,
														 &this->poBufferIBV_ia);
		assert(SUCCEEDED(hr));
		assert(this->poBufferIBV_ia);

	}

	void BufferIBV_ia::SetActive()
	{
		assert(bCreate == true);
		// slow down... check this function 
		StateDirectXMan::GetContext()->IASetIndexBuffer(this->poBufferIBV_ia,
														 DXGI_FORMAT_R32_UINT,
														 0);
	}

	BufferIBV_ia::~BufferIBV_ia()
	{
		assert(bCreate == true);
		SafeRelease(this->poBufferIBV_ia);
	}

}

//--- End of File ---
