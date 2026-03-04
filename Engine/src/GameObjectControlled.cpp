//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectControlled.h"
#include "GraphicsObject.h"

namespace Azul
{

	GameObjectControlled::GameObjectControlled(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject),
		index(-1)
	{

	}

	void GameObjectControlled::Update(AnimTime currentTime)
	{

	}


}

// --- End of File ---
