#ifndef STATE_VIEWPORT_H
#define STATE_VIEWPORT_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateViewport
	{
	public:
		StateViewport();
		StateViewport(const StateViewport &) = delete;
		StateViewport &operator = (const StateViewport &) = delete;
		virtual ~StateViewport();

		void Initialize(int _width, int _height);
		void Activate();

		D3D11_VIEWPORT *poD3DViewport;
	};

}
#endif 

// --- End of File ---
