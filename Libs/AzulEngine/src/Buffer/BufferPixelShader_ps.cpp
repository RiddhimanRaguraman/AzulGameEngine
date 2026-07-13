#include "BufferPixelShader_ps.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferPixelShader_ps::BufferPixelShader_ps(uint32_t _numBytes, void *pData)
		: codeSize { _numBytes },
		poPixelShader { nullptr },
		bCreate { false }
	{
		assert(pData);
		this->privCreate(pData);
	}

	void BufferPixelShader_ps::privCreate(void *pData)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		assert(pData);

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreatePixelShader(pData,
															  this->codeSize,
															  nullptr,
															  &this->poPixelShader);
		assert(SUCCEEDED(hr));
		assert(this->poPixelShader);


	}

	void BufferPixelShader_ps::SetActive()
	{
		// slow down... check this function 
		StateDirectXMan::GetContext()->PSSetShader(this->poPixelShader, nullptr, 0);
	}

	BufferPixelShader_ps::~BufferPixelShader_ps()
	{
		SafeRelease(this->poPixelShader);
	}

}

//--- End of File ---
