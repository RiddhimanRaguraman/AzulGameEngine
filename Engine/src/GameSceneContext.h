#ifndef GAME_SCENE_CONTEXT_H
#define GAME_SCENE_CONTEXT_H

#include "GameSceneState.h"

namespace Azul
{
	class Game;
	class Scene1;
	class Scene2;
	class Scene3;

	class GameSceneContext
	{
	public:
		enum class Scene
		{
			Scene1,
			Scene2,
			Scene3
		};

	public:
		GameSceneContext();
		GameSceneContext(const GameSceneContext&) = delete;
		GameSceneContext& operator=(const GameSceneContext&) = delete;
		~GameSceneContext();

		bool SetState(Game& game, Scene next);
		GameSceneState* GetState() const;
		Scene GetActiveScene() const;
		void Shutdown(Game& game);

	private:
		GameSceneState* pState;
		Scene1* pScene1;
		Scene2* pScene2;
		Scene3* pScene3;
		Scene activeScene;
		bool isLoaded;
	};
}

#endif
