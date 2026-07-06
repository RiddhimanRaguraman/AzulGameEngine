//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_ADAPTER_H
#define STATE_ADAPTER_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateAdapter
	{
	public:
		StateAdapter();
		StateAdapter(const StateAdapter &) = delete;
		StateAdapter &operator = (const StateAdapter &) = delete;
		virtual ~StateAdapter();

		IDXGIAdapter *poDXGAdapter;
	};

}
#endif 

// --- End of File ---
