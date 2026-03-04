//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_CONTROLLED_H
#define GAME_OBJECT_CONTROLLED_H

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObjectControlled : public GameObject
	{
	public:
		GameObjectControlled(GraphicsObject *pGraphicsObject);

		// Big four
		GameObjectControlled() = delete;
		GameObjectControlled(const GameObjectControlled &) = delete;
		GameObjectControlled &operator=(GameObjectControlled &) = delete;
		virtual ~GameObjectControlled() = default;

		virtual void Update(AnimTime currTime);
		virtual void SetIndex(int i) = 0;

	public:
		int index;

	};
}

#endif

// --- End of File ---
