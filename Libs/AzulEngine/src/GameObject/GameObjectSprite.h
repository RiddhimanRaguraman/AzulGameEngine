//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_SPRITE_H
#define GAME_OBJECT_SPRITE_H

#include "GameObjectRigidBody.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "Image.h"
#include "Rect.h"
#include "Color.h"
#include "TextureObject.h"
#include "AnimTime.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	struct Sprite2DComponent;

	// Data-path 2D sprite: no GraphicsObject. The mutable sprite state lives in the
	// entity's Sprite2DComponent; SpriteRenderSystem does the drawing. The 2D
	// transform (posX/posY/scale/angle) feeds the TransformComponent world.
	class AZUL_ENGINE_LIBRARY_API GameObjectSprite : public GameObjectRigidBody
	{
	public:
		GameObjectSprite(Mesh::Name mesh, ShaderObject::Name shader, Image::Name image, Rect rect);

		// Big four
		GameObjectSprite() = delete;
		GameObjectSprite(const GameObjectSprite &) = delete;
		GameObjectSprite &operator=(GameObjectSprite &) = delete;
		virtual ~GameObjectSprite();

		void SetImage(Image::Name image);
		void SetTexture(TextureObject *pText);
		void SetImage(Rect &imageRect, TextureObject *pText);
		void SetScreenRect(Rect &rect);
		void SetColor(Color &c);
		void SetColor(float r, float g, float b, float a);

		Sprite2DComponent &GetSprite2D();

		virtual void Update(AnimTime currTime) override;
		virtual void Draw() override;

	private:
		void privUpdate(AnimTime currTime);

	public:   // add accessors later
		float scaleX;
		float scaleY;
		float posX;
		float posY;
		float angle;
	};
}

#endif

// --- End of File ---
