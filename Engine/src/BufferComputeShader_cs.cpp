//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferComputeShader_cs.h"
#include "StateDirectXMan.h"
#include "ShaderObject.h"

namespace Azul
{
	BufferComputeShader_cs::BufferComputeShader_cs(size_t _numBytes, void *pData)
		: codeSize{_numBytes},
		poComputeShader{nullptr},
		bCreate{false}
	{
		assert(pData);
		this->privCreate(pData);
	}

	void BufferComputeShader_cs::privCreate(void *pData)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		assert(pData);

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateComputeShader(pData,
																this->codeSize,
																nullptr,
																&this->poComputeShader);
		assert(SUCCEEDED(hr));
		assert(this->poComputeShader);

		assert(std::string("cs_5_0") == (ShaderObject::GetLatestProfile_ComputeShader())); 
	}

	void BufferComputeShader_cs::SetActive()
	{
		// slow down... check this function (VS,CS,PS) types
		StateDirectXMan::GetContext()->CSSetShader(this->poComputeShader, nullptr, 0);
	}

	BufferComputeShader_cs::~BufferComputeShader_cs()
	{
		SafeRelease(this->poComputeShader);
	}

}

//--- End of File ---
