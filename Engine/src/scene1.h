#ifndef SCENE1_H
#define SCENE1_H

#include "GameSceneState.h"

namespace Azul
{
	class Scene1 final : public GameSceneState
	{
	public:
		Scene1() = default;
		Scene1(const Scene1&) = delete;
		Scene1& operator=(const Scene1&) = delete;
		~Scene1() override = default;

		bool Load(Game& game) override;
		void Update(Game& game, AnimTime tCurr, AnimTime tDelta) override;
		void Unload(Game& game) override;
	};
}

#endif

