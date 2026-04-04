#include "GameSceneContext.h"

#include "scene1.h"
#include "scene2.h"
#include "scene3.h"

namespace Azul
{
	GameSceneContext::GameSceneContext()
		: pState(nullptr),
		pScene1(new Scene1()),
		pScene2(new Scene2()),
		pScene3(new Scene3()),
		activeScene(Scene::Scene1),
		isLoaded(false)
	{
	}

	GameSceneContext::~GameSceneContext()
	{
		delete this->pScene3;
		this->pScene3 = nullptr;

		delete this->pScene2;
		this->pScene2 = nullptr;

		delete this->pScene1;
		this->pScene1 = nullptr;

		this->pState = nullptr;
	}

	bool GameSceneContext::SetState(Game& game, Scene next)
	{
		if (this->isLoaded && this->pState && next == this->activeScene)
		{
			return true;
		}

		if (this->isLoaded && this->pState)
		{
			this->pState->Unload(game);
			this->isLoaded = false;
		}

		switch (next)
		{
		case Scene::Scene1:
			this->pState = this->pScene1;
			break;

		case Scene::Scene2:
			this->pState = this->pScene2;
			break;

		case Scene::Scene3:
			this->pState = this->pScene3;
			break;

		default:
			this->pState = this->pScene1;
			next = Scene::Scene1;
			break;
		}

		this->activeScene = next;
		this->isLoaded = this->pState->Load(game);
		return this->isLoaded;
	}

	GameSceneState* GameSceneContext::GetState() const
	{
		return this->pState;
	}

	GameSceneContext::Scene GameSceneContext::GetActiveScene() const
	{
		return this->activeScene;
	}

	void GameSceneContext::Shutdown(Game& game)
	{
		if (this->isLoaded && this->pState)
		{
			this->pState->Unload(game);
			this->isLoaded = false;
		}
	}
}
