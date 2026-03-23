#ifndef SCENE2_H
#define SCENE2_H

#include "GameSceneState.h"

namespace Azul
{
	class Scene2 final : public GameSceneState
	{
	public:
		Scene2() = default;
		Scene2(const Scene2&) = delete;
		Scene2& operator=(const Scene2&) = delete;
		~Scene2() override = default;

		bool Load(Game& game) override;
		void Update(Game& game, AnimTime tCurr, AnimTime tDelta) override;
		void Unload(Game& game) override;
	};
}

#endif

