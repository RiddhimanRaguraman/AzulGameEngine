//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Engine.h"
#include "AnimTimer.h"
#include "StateDirectXMan.h"
#include "BufferTexture2D.h"
#include "StateDebug.h"

namespace Azul
{
	LPCSTR g_WindowClassName = "EngineWindowClass";

	Engine::Engine(const char *_pName, int _width, int _height)
		:
		mWindowHandle(0),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mStateDepthStencil(),
		mBlendStateOff(),
		mBlendStateAlpha(), 
		mStateRasterizerSolidCull(),
		mStateRasterizerWireNoCull(),
		mStateRasterizerWireCull(),
		mViewport(),
		pName(_pName),
		mWindowWidth(_width),
		mWindowHeight(_height)
	{
	}

	// --------------------------------------------------------------
	//  Standard windows magic to setup the application window
	// --------------------------------------------------------------
	int Engine::InitApplication(HINSTANCE hInstance, int cmdShow)
	{
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = this->WndProc;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hIcon = nullptr;
		wndClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = g_WindowClassName;
		wndClass.hIconSm = nullptr;

		if (!RegisterClassEx(&wndClass))
		{
			return -1;
		}

		RECT windowRect = { 0, 0, this->mWindowWidth, this->mWindowHeight };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		mWindowHandle = CreateWindowA(g_WindowClassName, this->pName,
									  WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
									  windowRect.right - windowRect.left,
									  windowRect.bottom - windowRect.top,
									  nullptr, nullptr, hInstance, nullptr);

		assert(mWindowHandle);

		ShowWindow(mWindowHandle, cmdShow);
		UpdateWindow(mWindowHandle);

		return 0;
	}

	// ------------------------------------------------------------
	//  Initialize the Direct Device with Swap Chain
	// ------------------------------------------------------------

	int Engine::InitDirectX(HINSTANCE hInstance, BOOL vSync)
	{
		AZUL_UNUSED_VAR(hInstance);
		// A window handle must have been created already.
		assert(mWindowHandle != nullptr);

		// Create Device and SwapChain
		StateDirectXMan::Create(mWindowHandle, vSync);

		// The Direct3D device and swap chain were successfully created.
		// Now we need to initialize the buffers of the swap chain.
		// Next initialize the back buffer of the swap chain and associate it to a 
		// render target view.
		this->mStateRenderTargetView.Initialize();

		// Create the depth buffer for use with the depth/stencil view.
		this->mDepthStencilBuffer.Initialize(this->mWindowWidth, this->mWindowHeight);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);

		// Setup depth/stencil state.
		this->mStateDepthStencil.Initialize();

		// Setup blends state
		this->mBlendStateOff.Initialize(StateBlend::Mode::AlphaDisabled);

		// Disables Blending.... 
		this->mBlendStateAlpha.Initialize(StateBlend::Mode::AlphaEnabled);

		// Setup rasterizer state.
		this->mStateRasterizerSolidCull.Initialize(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
		this->mStateRasterizerWireNoCull.Initialize(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
		this->mStateRasterizerWireCull.Initialize(D3D11_FILL_WIREFRAME, D3D11_CULL_FRONT);

		// Initialize the viewport to occupy the entire client area.
		this->mViewport.Initialize(this->mWindowWidth, this->mWindowHeight);

		return 0;
	}

	// ------------------------------------
	// win main()
	// ------------------------------------
	int WINAPI Engine::wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
	{
		AZUL_UNUSED_VAR(prevInstance);
		AZUL_UNUSED_VAR(cmdLine);

		if (InitApplication(hInstance, cmdShow) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create applicaiton window."), TEXT("Error"), MB_OK);
			return -1;
		}

		if (InitDirectX(hInstance, ENABLE_VSYNC) != 0)
		{
			MessageBox(nullptr, TEXT("Failed to create DirectX device and swap chain."), TEXT("Error"), MB_OK);
			return -1;
		}

		int returnCode = Run();

		return returnCode;
	}

	// ------------------------------------
	// windows process - messages
	// ------------------------------------
	LRESULT CALLBACK Engine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		switch (message)
		{
			case WM_PAINT:
			{
				hDC = BeginPaint(hwnd, &paintStruct);
				EndPaint(hwnd, &paintStruct);
			}
			break;

			case WM_CHAR:
			{
				if (wParam == VK_ESCAPE)
				{
					PostQuitMessage(0);
				}
			}
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;

			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}

	// ------------------------------------
	// Run - main loop
	// ------------------------------------
	int Engine::Run()
	{
		MSG msg = { 0 };

		AnimTimer EngineTime;

		if (!LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return -1;
		}

		while (msg.message != WM_QUIT)
		{
			EngineTime.Tic();

			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Update();
				ClearDepthStencilBuffer();
				Render();

				//--------------------------------
				// Fast monitor sync
				//--------------------------------
				{
					AnimTime EngineLoopTime = EngineTime.Toc();

					// Current number of Frames... snap to the next 60Hz frame as target
					int NumFrames_60Hz = AnimTime::Quotient(EngineLoopTime, AnimTime(AnimTime::Duration::NTSC_FRAME));
					int TargetNumFrames_60Hz = NumFrames_60Hz + 1;
					AnimTime Target_60Hz = TargetNumFrames_60Hz * AnimTime(AnimTime::Duration::NTSC_FRAME);

					// we are before the flip... give a little cushion 
					AnimTime Overhead_Time = 500 * AnimTime(AnimTime::Duration::ONE_MICROSECOND);
					AnimTime EndTime = Target_60Hz - Overhead_Time;

					// Sit and spin.
					while (EngineLoopTime < EndTime)
					{
						EngineLoopTime = EngineTime.Toc();
					}
				}

				//--------------------------------------------------------
				// Wait for Vsync - flip front/back buffers
				//--------------------------------------------------------
				Present(ENABLE_VSYNC);
			}
		}

		UnloadContent();
		Cleanup();

		return static_cast<int>(msg.wParam);
	}

	// ------------------------------------
	// present - flip
	// ------------------------------------
	void Engine::Present(bool vSync)
	{
		if (vSync)
		{
			StateDirectXMan::GetSwapChain()->Present(1, 0);
		}
		else
		{
			StateDirectXMan::GetSwapChain()->Present(0, 0);
		}
	}

	// ------------------------------------
	// Cleanup()
	// ------------------------------------
	void Engine::Cleanup()
	{
#ifdef _DEBUG
		HRESULT hr = S_OK;

		// Now we set up the Debug interface, to be queried on shutdown
		StateDebug DebugState;
		hr = StateDirectXMan::GetDevice()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void **>(&DebugState.poD3DDebug));

		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);

#endif
	}

	Engine::~Engine()
	{
		StateDirectXMan::Destroy();
	}
}

// --- End of File ---
