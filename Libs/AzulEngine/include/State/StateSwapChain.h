//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef STATE_SWAP_CHAIN_H
#define STATE_SWAP_CHAIN_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateSwapChain
	{
	public:
		StateSwapChain();
		StateSwapChain(const StateSwapChain &) = delete;
		StateSwapChain &operator = (const StateSwapChain &) = delete;
		virtual ~StateSwapChain();


		IDXGISwapChain *poD3DSwapChain;
	};

}
#endif 

// --- End of File ---
