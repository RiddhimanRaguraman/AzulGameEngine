//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferVBV.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferVBV::BufferVBV()
		: numBytes { 0},
		strideSize {0 },
		offset { 0 },
		poBufferVBV { nullptr },
		bCreate { false }
	{
		assert(bCreate == false);
	}

	void BufferVBV::Initialize(uint32_t _numBytes, uint32_t _strideSize, void *pData)
	{
		this->numBytes = _numBytes;
		this->strideSize = _strideSize;
		assert(bCreate == false);

		assert(pData);
		this->privCreate(pData);
	}

	void BufferVBV::privCreate(void *pData)
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
		hr = StateDirectXMan::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, &this->poBufferVBV);

		assert(SUCCEEDED(hr));
		assert(this->poBufferVBV);
	}

	void BufferVBV::SetActive(VertexSlot slot)
	{
		if(bCreate)
		{
		// slow down... check this function 
		StateDirectXMan::GetContext()->IASetVertexBuffers((uint32_t) slot,
														   1,
														   &this->poBufferVBV,
														   &this->strideSize,
														   &this->offset);

		}
	}

	BufferVBV::~BufferVBV()
	{
		// sometimes - color is not part of mesh
		//             so it null
		SafeRelease(this->poBufferVBV);
	}

}

//--- End of File ---
