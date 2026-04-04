#ifndef SCENE3_H
#define SCENE3_H

#include "GameSceneState.h"

namespace Azul
{
	class Scene3 final : public GameSceneState
	{
	public:
		Scene3() = default;
		Scene3(const Scene3&) = delete;
		Scene3& operator=(const Scene3&) = delete;
		~Scene3() override = default;

		bool Load(Game& game) override;
		void Update(Game& game, AnimTime tCurr, AnimTime tDelta) override;
		void Unload(Game& game) override;
	};
}

#endif
