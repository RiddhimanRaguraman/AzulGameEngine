#ifndef STATE_RASTERIZER_H
#define STATE_RASTERIZER_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateRasterizer
	{
	public:
		StateRasterizer();
		StateRasterizer(const StateRasterizer &) = delete;
		StateRasterizer &operator = (const StateRasterizer &) = delete;
		virtual ~StateRasterizer();

		void Initialize(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode);
		
		void Activate();

		ID3D11RasterizerState *poD3DRasterizerState;
	};

}
#endif 

// --- End of File ---
