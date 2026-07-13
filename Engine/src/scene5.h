#ifndef SCENE5_H
#define SCENE5_H

#include "GameSceneState.h"

namespace Azul
{
	// Phase 6 STRESS scene: a 10x10x10 grid of 1000 cubes, all sharing ONE mesh +
	// ONE material (ColorByVertex), each spun by the ECS RotateComponent. Drives
	// the systems loop + render pass at scale so the Profiler numbers are
	// meaningful and to set up / demonstrate GPU instancing (1000 draw calls -> 1).
	// Press 5 to activate.
	class Scene5 final : public GameSceneState
	{
	public:
		Scene5() = default;
		Scene5(const Scene5&) = delete;
		Scene5& operator=(const Scene5&) = delete;
		~Scene5() override = default;

		bool Load(Game& game) override;
		void Update(Game& game, AnimTime tCurr, AnimTime tDelta) override;
		void Unload(Game& game) override;
	};
}

#endif
