#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Colors.h"
#include "AnimTimer.h"

namespace Azul
{
	class GameSceneContext;

	class Game : public Engine
	{
	public:

		Game(const char *const pName, int width, int height);

		Game() = delete;
		Game(const Game &) = delete;
		Game &operator = (const Game &) = delete;
		virtual ~Game();

		virtual bool LoadContent() override;
		virtual void UnloadContent() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual	void ClearDepthStencilBuffer() override;

		// Rendering
		float GetAspectRatio() const;
		void SetDefaultTargetMode();

		AnimTimer	intervalTimer;
		AnimTimer	globalTimer;

	private:
		GameSceneContext* pSceneContext;
	};
}

#endif

// --- End of File ---

