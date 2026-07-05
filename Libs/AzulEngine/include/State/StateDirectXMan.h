//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
#ifndef STATE_DIRECT_X_MAN_H
#define STATE_DIRECT_X_MAN_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

#include "StateDevice.h"
#include "StateDeviceContext.h"
#include "StateSwapChain.h"
#include "SafeRelease.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateDirectXMan
	{
	public:

		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------

		static void Create(HWND g_WindowHandle, bool vSync);
		static void Destroy();

		static ID3D11Device *GetDevice();
		static ID3D11DeviceContext *GetContext();
		static IDXGISwapChain *GetSwapChain();

		~StateDirectXMan();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static StateDirectXMan *privGetInstance();

		StateDirectXMan();
		StateDirectXMan(const StateDirectXMan &) = delete;
		StateDirectXMan &operator = (const StateDirectXMan &) = delete;

		static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);


		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		static StateDirectXMan *posInstance;
		StateDevice        mD3DDevice;
		StateDeviceContext mD3DDeviceContext;
		StateSwapChain     mD3DSwapChain;

	};
}

#endif

// --- End of File ---
