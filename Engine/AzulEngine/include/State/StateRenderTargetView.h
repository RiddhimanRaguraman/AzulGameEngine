//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_RENDER_TARGET_VIEW_H
#define STATE_RENDER_TARGET_VIEW_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "MathEngine.h"
#include "StateDepthStencilView.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateRenderTargetView
	{
	public:
		StateRenderTargetView();
		StateRenderTargetView(const StateRenderTargetView &) = delete;
		StateRenderTargetView &operator = (const StateRenderTargetView &) = delete;
		virtual ~StateRenderTargetView();

		void Initialize();
		void Clear(const Vec4 vRGBAColor);
		void Activate(StateDepthStencilView &r);

	private:

		void privInitialize(ID3D11Texture2D *backBuffer);

		ID3D11RenderTargetView *poD3DRenderTargetView;
	};

}
#endif 

// --- End of File ---
