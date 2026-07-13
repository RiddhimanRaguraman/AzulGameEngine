#include "BufferVertexShader.h"
#include "StateDirectXMan.h"

namespace Azul
{
	BufferVertexShader::BufferVertexShader(uint32_t _numBytes, void *pData)
		: codeSize { _numBytes },
		poVertexShader { nullptr },
		bCreate { false }
	{
		assert(pData);
		this->privCreate(pData);
	}

	void BufferVertexShader::privCreate(void *pData)
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		assert(pData);

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateVertexShader(pData,
															   this->codeSize,
															   nullptr,
															   &this->poVertexShader);
		assert(SUCCEEDED(hr));
		assert(this->poVertexShader);

	}

	void BufferVertexShader::SetActive()
	{
		// slow down... check this function 
		StateDirectXMan::GetContext()->VSSetShader(this->poVertexShader, nullptr, 0);
	}

	BufferVertexShader::~BufferVertexShader()
	{
		SafeRelease(this->poVertexShader);
	}

}

//--- End of File ---
