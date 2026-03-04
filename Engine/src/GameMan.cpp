//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "GameMan.h"
#include "Engine.h"

namespace Azul
{
	GameMan *GameMan::pInstance = nullptr;

	int GameMan::Create(const char *windowName, 
						const int Width, 
						const int Height,
						HINSTANCE hInstance, 
						HINSTANCE prevInstance, 
						LPWSTR cmdLine, 
						int cmdShow)
	{
		GameMan::privCreate(windowName, Width, Height);

		// Launch the window
		int status = GameMan::privInstance()->poGame->wWinMain(hInstance, prevInstance, cmdLine, cmdShow);

		return status;
	}

	void GameMan::Destroy()
	{
		GameMan *pMan = GameMan::privInstance();
		assert(pMan);

		assert(pMan->poGame);
		delete pMan->poGame;
		pMan->poGame = nullptr;
	}

	void GameMan::privCreate(const char *windowName, const int Width, const int Height)
	{
		Game *poGame = new Game(windowName, Width, Height);

		// Storage
		static GameMan gameMan(poGame);
		GameMan::pInstance = &gameMan;
	}

	GameMan::GameMan(Game *poGame)
	{
		assert(poGame);
		this->poGame = poGame;
	}

	GameMan::~GameMan()
	{
		assert(this->poGame == nullptr);
	}

	Game *GameMan::GetGame()
	{
		GameMan *pMan = GameMan::privInstance();
		assert(pMan);
		assert(pMan->poGame);
		return pMan->poGame;
	}

	GameMan *GameMan::privInstance()
	{
		assert(GameMan::pInstance);
		return GameMan::pInstance;
	}
}

// --- End of File ---
