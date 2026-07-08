#ifndef SCENE4_H
#define SCENE4_H

#include "GameSceneState.h"

namespace Azul
{
	// Phase 3 demo scene: a single cube spun purely by the ECS RotateComponent +
	// RotateSystem (no Prefab, no per-object Update logic). Press 4 to activate.
	class Scene4 final : public GameSceneState
	{
	public:
		Scene4() = default;
		Scene4(const Scene4&) = delete;
		Scene4& operator=(const Scene4&) = delete;
		~Scene4() override = default;

		bool Load(Game& game) override;
		void Update(Game& game, AnimTime tCurr, AnimTime tDelta) override;
		void Unload(Game& game) override;
	};
}

#endif
