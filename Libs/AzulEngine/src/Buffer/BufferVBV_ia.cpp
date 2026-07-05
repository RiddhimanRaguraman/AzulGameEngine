//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferVBV_ia.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferVBV_ia::BufferVBV_ia()
		: numBytes { 0},
		strideSize {0 },
		offset { 0 },
		poBufferVBV_ia { nullptr },
		bCreate { false }
	{
		assert(bCreate == false);
	}

	void BufferVBV_ia::Initialize(uint32_t _numBytes, uint32_t _strideSize, void *pData)
	{
		this->numBytes = _numBytes;
		this->strideSize = _strideSize;
		assert(bCreate == false);

		assert(pData);
		this->privCreate(pData);
	}

	void BufferVBV_ia::privCreate(void *pData)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		assert(pData);

		// Create an initialize the vertex buffer.
		D3D11_BUFFER_DESC vertexBufferDesc { 0 };
		D3D11_SUBRESOURCE_DATA resourceData { 0 };

		// type of buffer
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = this->numBytes;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		resourceData.pSysMem = pData;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, &this->poBufferVBV_ia);

		assert(SUCCEEDED(hr));
		assert(this->poBufferVBV_ia);
	}

	void BufferVBV_ia::SetActive(VertexSlot slot)
	{
		if(bCreate)
		{
		// slow down... check this function 
		StateDirectXMan::GetContext()->IASetVertexBuffers((uint32_t) slot,
														   1,
														   &this->poBufferVBV_ia,
														   &this->strideSize,
														   &this->offset);

		}
	}

	BufferVBV_ia::~BufferVBV_ia()
	{
		// sometimes - color is not part of mesh
		//             so it null
		SafeRelease(this->poBufferVBV_ia);
	}

}

//--- End of File ---
