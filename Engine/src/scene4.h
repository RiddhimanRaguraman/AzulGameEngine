#ifndef SCENE4_H
#define SCENE4_H

#include "GameSceneState.h"

namespace Azul
{
	// Demo scene: three cubes spun by the RotateComponent + RotateSystem, one per
	// material (ColorByVertex / ConstColorLight / Wireframe). Press 4 to activate.
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
