#ifndef STATE_DEPTH_STENCIL_H
#define STATE_DEPTH_STENCIL_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDepthStencil
	{
	public:
		StateDepthStencil();
		StateDepthStencil(const StateDepthStencil &) = delete;
		StateDepthStencil &operator = (const StateDepthStencil &) = delete;
		virtual ~StateDepthStencil();

		void Initialize();
		void Activate();

		ID3D11DepthStencilState *poD3DDepthStencilState;
	};

}
#endif 

// --- End of File ---
