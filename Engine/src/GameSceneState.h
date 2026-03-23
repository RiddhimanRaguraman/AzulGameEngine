#ifndef GAME_SCENE_STATE_H
#define GAME_SCENE_STATE_H

#include "AnimTime.h"

namespace Azul
{
	class Game;

	class GameSceneState
	{
	public:
		GameSceneState() = default;
		GameSceneState(const GameSceneState&) = delete;
		GameSceneState& operator=(const GameSceneState&) = delete;
		virtual ~GameSceneState() = default;

		virtual bool Load(Game& game) = 0;
		virtual void Update(Game& game, AnimTime tCurr, AnimTime tDelta) = 0;
		virtual void Unload(Game& game) = 0;
	};
}

#endif

