//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferSamplerState.h"
#include "StateDirectXMan.h"

namespace Azul
{

	BufferSamplerState::BufferSamplerState()
		: mFilter{D3D11_FILTER_MIN_MAG_MIP_LINEAR},
		poSampler{nullptr},
		bCreate{false}
	{
	}

	void BufferSamplerState::Initialize(D3D11_FILTER filter)
	{
		assert(mFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);
		mFilter = filter;
		assert(poSampler == nullptr);
		assert(bCreate == false);

		this->privCreate();
	}

	void BufferSamplerState::privCreate()
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		// Create texture
		HRESULT hr;

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = mFilter;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = StateDirectXMan::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);
		assert(SUCCEEDED(hr));
		assert(poSampler);
	}

	void BufferSamplerState::SetActive(ShaderSamplerSlot slot)
	{
		assert(bCreate == true);
		StateDirectXMan::GetContext()->PSSetSamplers((uint32_t)slot,
													  1,
													  &poSampler);

	}

	BufferSamplerState::~BufferSamplerState()
	{
		SafeRelease(this->poSampler);
	}

}

//--- End of File ---
