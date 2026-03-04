//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_SPRITE_H
#define GAME_OBJECT_SPRITE_H

#include "GameObjectRigidBody.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObjectSprite : public GameObjectRigidBody
	{
	public:
		GameObjectSprite(GraphicsObject *graphicsObject);

		// Big four
		GameObjectSprite() = delete;
		GameObjectSprite(const GameObjectSprite &) = delete;
		GameObjectSprite &operator=(GameObjectSprite &) = delete;
		virtual ~GameObjectSprite();

		void SetImage(Image::Name image);

		virtual void Update(AnimTime currTime);

	private:
		void privUpdate(AnimTime currTime);

	public:   // add accessors later
		float scaleX;
		float scaleY;
		float posX;
		float posY;
		float angle;

		// Alias
		GraphicsObject_Sprite* pGraphicsObjectSprite;

	};
}

#endif

// --- End of File ---
