//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_DEVICE_CONTEXT_H
#define STATE_DEVICE_CONTEXT_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDeviceContext
	{
	public:
		StateDeviceContext();
		StateDeviceContext(const StateDeviceContext &) = delete;
		StateDeviceContext &operator = (const StateDeviceContext &) = delete;
		virtual ~StateDeviceContext();

		

		ID3D11DeviceContext *poD3DDeviceContext;
	};

}
#endif 

// --- End of File ---
