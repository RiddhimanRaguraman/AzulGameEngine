#ifndef ENGINE_H
#define ENGINE_H

#include "EngineDLLInterface.h"
#include <d3d11.h>
#include "StateDepthStencil.h"
#include "StateRasterizer.h"
#include "StateBlend.h"
#include "StateViewport.h"
#include "StateDepthStencilBuffer.h"
#include "StateDepthStencilView.h"
#include "StateRenderTargetView.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API Engine
	{
	public:
		static const BOOL ENABLE_VSYNC = true;

		// DLL-side access to the running engine instance (set in the ctor).
		// Lets engine code in the DLL reach the shared render states without
		// going through the app-side GameMan/Game.
		static Engine *GetInstance();

	public:
		Engine(const char *pName, int width, int height);

		Engine() = delete;
		Engine(const Engine &) = delete;
		Engine &operator = (const Engine &) = delete;
		virtual ~Engine();

		// Game overloads - required
		virtual bool LoadContent() = 0;
		virtual void UnloadContent() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual	void ClearDepthStencilBuffer() = 0;

		void Cleanup();

		int InitApplication(HINSTANCE hInstance, int cmdShow);
		int InitDirectX(HINSTANCE hInstance, BOOL vSync);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);

		void Present(bool vSync);
		int Run();

		HWND mWindowHandle;

		StateRenderTargetView mStateRenderTargetView;
		StateDepthStencilView mDepthStencilView;
		StateDepthStencilBuffer mDepthStencilBuffer;
		StateBlend mBlendStateOff;
		StateBlend mBlendStateAlpha;
		StateDepthStencil mStateDepthStencil;
		StateRasterizer mStateRasterizerSolidCull;
		StateRasterizer mStateRasterizerWireNoCull;
		StateRasterizer mStateRasterizerWireCull;
		StateViewport mViewport;

		const char *pName;
		const int  mWindowWidth;
		const int  mWindowHeight;

	private:
		static Engine *posInstance;
	};

}

#endif

// --- End of File ---
