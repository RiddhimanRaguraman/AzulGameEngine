#ifndef STATE_DEBUG_H
#define STATE_DEBUG_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDebug
	{
	public:
		StateDebug();
		StateDebug(const StateDebug &) = delete;
		StateDebug &operator = (const StateDebug &) = delete;
		virtual ~StateDebug();

		ID3D11Debug *poD3DDebug;
	};

}
#endif 

// --- End of File ---
