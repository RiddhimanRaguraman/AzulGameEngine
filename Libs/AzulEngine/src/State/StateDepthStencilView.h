#ifndef STATE_DEPTH_STENCIL_VIEW_H
#define STATE_DEPTH_STENCIL_VIEW_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "StateDepthStencilBuffer.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDepthStencilView
	{
	public:
		StateDepthStencilView();
		StateDepthStencilView(const StateDepthStencilView &) = delete;
		StateDepthStencilView &operator = (const StateDepthStencilView &) = delete;
		virtual ~StateDepthStencilView();

		void Initialize(StateDepthStencilBuffer &r);
		void Clear(float clearDepth,uint8_t clearStencil);

		ID3D11DepthStencilView *poD3DDepthStencilView;
	};

}
#endif 

// --- End of File ---
