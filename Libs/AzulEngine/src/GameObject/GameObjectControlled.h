//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_CONTROLLED_H
#define GAME_OBJECT_CONTROLLED_H

#include "GameObject.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API GameObjectControlled : public GameObject
	{
	public:
		GameObjectControlled(MaterialKind kind);

		// Big four
		GameObjectControlled() = delete;
		GameObjectControlled(const GameObjectControlled &) = delete;
		GameObjectControlled &operator=(GameObjectControlled &) = delete;
		virtual ~GameObjectControlled() = default;

		virtual void Update(AnimTime currTime);

	public:
		int index;

	};
}

#endif

// --- End of File ---
