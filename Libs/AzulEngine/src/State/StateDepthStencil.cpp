//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3dcompiler.h>
#include "StateDepthStencil.h"
#include "StateDirectXMan.h"

namespace Azul
{
	StateDepthStencil::StateDepthStencil()
		:poD3DDepthStencilState{nullptr}
	{
	}

	void StateDepthStencil::Initialize()
	{
		// Setup depth/stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{0};

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// not using any stencil... its disabled
		depthStencilStateDesc.StencilEnable = FALSE;

		HRESULT hr;
		hr = StateDirectXMan::GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &this->poD3DDepthStencilState);
		assert(SUCCEEDED(hr));
	}

	void StateDepthStencil::Activate()
	{
		assert(this->poD3DDepthStencilState);
		StateDirectXMan::GetContext()->OMSetDepthStencilState(this->poD3DDepthStencilState, 1);
	}

	StateDepthStencil::~StateDepthStencil()
	{
		SafeRelease(this->poD3DDepthStencilState);
		this->poD3DDepthStencilState = nullptr;
	}

}

// --- End of File ---
