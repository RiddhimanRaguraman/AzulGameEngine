#ifndef SCENE5_H
#define SCENE5_H

#include "GameSceneState.h"

namespace Azul
{
	// Stress scene: a grid of cubes (kGrid^3), all sharing ONE mesh + ONE material
	// (ColorByVertex), each spun by a RotateComponent. Drives the systems loop +
	// render pass at scale. Press 5 to activate.
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
