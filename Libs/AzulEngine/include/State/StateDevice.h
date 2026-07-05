//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_DEVICE_H
#define STATE_DEVICE_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDevice
	{
	public:
		StateDevice();
		StateDevice(const StateDevice &) = delete;
		StateDevice &operator = (const StateDevice &) = delete;
		virtual ~StateDevice();


		ID3D11Device *poD3DDevice;
	};

}
#endif 

// --- End of File ---
