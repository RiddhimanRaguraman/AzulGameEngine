//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Game.h"

#include "GameSceneContext.h"
#include "GameObjectMan.h"

 
namespace Azul
{
	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------

	Game::Game(const char *const pName, int width, int height)
		: Engine(pName, width, height)
		, pSceneContext(nullptr)
	{
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	//  Game::~Game()
	//		Game Engine destructor
	//-----------------------------------------------------------------------------
	Game::~Game()
	{
		Trace::out("\n");
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
		assert(this->pSceneContext == nullptr);
		this->pSceneContext = new GameSceneContext();
		return this->pSceneContext->SetState(*this, GameSceneContext::Scene::Scene1);
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------

    void Game::Update()
    {

		AnimTime tDelta = this->intervalTimer.Toc();
		this->intervalTimer.Tic();

		// capture time
		AnimTime tCurr = this->globalTimer.Toc();

		if (this->pSceneContext)
		{
			const SHORT oneState = GetAsyncKeyState('1');
			if (oneState & 0x0001)
			{
				this->pSceneContext->SetState(*this, GameSceneContext::Scene::Scene1);
			}

			const SHORT twoState = GetAsyncKeyState('2');
			if (twoState & 0x0001)
			{
				this->pSceneContext->SetState(*this, GameSceneContext::Scene::Scene2);
			}

			const SHORT threeState = GetAsyncKeyState('3');
			if (threeState & 0x0001)
			{
				this->pSceneContext->SetState(*this, GameSceneContext::Scene::Scene3);
			}

			const SHORT fourState = GetAsyncKeyState('4');
			if (fourState & 0x0001)
			{
				this->pSceneContext->SetState(*this, GameSceneContext::Scene::Scene4);
			}

			GameSceneState* pState = this->pSceneContext->GetState();
			if (pState)
			{
				pState->Update(*this, tCurr, tDelta);
			}
		}

	}


	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		this->SetDefaultTargetMode();

		GameObjectMan::Draw();
	}

	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnloadContent()
	{
		if (this->pSceneContext)
		{
			this->pSceneContext->Shutdown(*this);
			delete this->pSceneContext;
			this->pSceneContext = nullptr;
		}

	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	// 
	// Clear the color and depth buffers.
	//------------------------------------------------------------------
	void Game::ClearDepthStencilBuffer()
	{
#ifdef _DEBUG
		this->mStateRenderTargetView.Clear(Azul::Colors::LightGray);
		this->mDepthStencilView.Clear(1.0f, 0);
#else
		this->mStateRenderTargetView.Clear(Azul::Colors::Wheat);
		this->mDepthStencilView.Clear(1.0f, 0);
#endif

	}

}

// --- End of File ---
