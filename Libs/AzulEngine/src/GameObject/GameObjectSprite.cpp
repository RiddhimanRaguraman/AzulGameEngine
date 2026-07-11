//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectSprite.h"
#include "Mesh.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "ImageMan.h"
#include "WorldMan.h"
#include "World.h"
#include "Sprite2DComponent.h"
#include "SpriteRenderSystem.h"

namespace Azul
{
	GameObjectSprite::GameObjectSprite(Mesh::Name mesh, ShaderObject::Name shader, Image::Name imageName, Rect rect)
		: GameObjectRigidBody(MaterialKind::Sprite),
		scaleX(1.0f),
		scaleY(1.0f),
		posX(0.0f),
		posY(0.0f),
		angle(0.0f)
	{
		// Fill the 2D-sprite render state (was GraphicsObject_Sprite's ctor).
		Sprite2DComponent& s = WorldMan::GetWorld().Add<Sprite2DComponent>(this->GetEntity());

		s.pMesh = MeshNodeMan::Find(mesh);
		s.pShader = ShaderObjectNodeMan::Find(shader);
		assert(s.pMesh);
		assert(s.pShader);

		Image* pImage = ImageMan::Find(imageName);
		assert(pImage);
		s.pTexture = pImage->pText;

		// UV matrix (sub-image within the texture).
		float w = pImage->imageRect.width / (float)pImage->pText->width;
		float h = pImage->imageRect.height / (float)pImage->pText->height;
		float u = pImage->imageRect.x / (float)pImage->pText->width;
		float v = pImage->imageRect.y / (float)pImage->pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		s.uvMatrix = ScaleUV * TransUV;

		// Orig (screen-rect scale) matrix.
		Scale ScaleRect(rect.width, rect.height, 1.0f);
		s.origMatrix = ScaleRect;

		s.color.Set(1.0f, 1.0f, 1.0f, 1.0f);

		this->posX = rect.x;
		this->posY = rect.y;
	}

	GameObjectSprite::~GameObjectSprite()
	{
	}

	Sprite2DComponent& GameObjectSprite::GetSprite2D()
	{
		Sprite2DComponent* pS = WorldMan::GetWorld().TryGet<Sprite2DComponent>(this->GetEntity());
		assert(pS);
		return *pS;
	}

	void GameObjectSprite::SetImage(Image::Name imageName)
	{
		Image* pImage = ImageMan::Find(imageName);
		assert(pImage);

		Sprite2DComponent& s = this->GetSprite2D();
		s.pTexture = pImage->pText;

		float w = pImage->imageRect.width / (float)pImage->pText->width;
		float h = pImage->imageRect.height / (float)pImage->pText->height;
		float u = pImage->imageRect.x / (float)pImage->pText->width;
		float v = pImage->imageRect.y / (float)pImage->pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		s.uvMatrix = ScaleUV * TransUV;
	}

	void GameObjectSprite::SetTexture(TextureObject* pText)
	{
		assert(pText);
		this->GetSprite2D().pTexture = pText;
	}

	void GameObjectSprite::SetImage(Rect& imageRect, TextureObject* pText)
	{
		assert(pText);

		float w = imageRect.width / (float)pText->width;
		float h = imageRect.height / (float)pText->height;
		float u = imageRect.x / (float)pText->width;
		float v = imageRect.y / (float)pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		this->GetSprite2D().uvMatrix = ScaleUV * TransUV;
	}

	void GameObjectSprite::SetScreenRect(Rect& rect)
	{
		Scale ScaleRect(rect.width, rect.height, 1.0f);
		this->GetSprite2D().origMatrix = ScaleRect;
	}

	void GameObjectSprite::SetColor(Color& c)
	{
		this->GetSprite2D().color.Set(c);
	}

	void GameObjectSprite::SetColor(float r, float g, float b, float a)
	{
		this->GetSprite2D().color.Set(r, g, b, a);
	}

	void GameObjectSprite::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		Scale S(this->scaleX, this->scaleY, 1.0f);
		Rot R(Rot1::Z, this->angle);
		Trans T(this->posX, this->posY, 0.0f);

		this->GetTransform().world = S * R * T;
	}

	void GameObjectSprite::Update(AnimTime currentTime)
	{
		this->privUpdate(currentTime);
	}

	void GameObjectSprite::Draw()
	{
		// Data-path: SpriteRenderSystem draws one 2D quad from the Sprite2DComponent
		// + the current world. (FontSprite::Draw calls this per glyph.)
		SpriteRenderSystem::DrawSprite(this->GetSprite2D(), this->GetTransform().world);
	}
}

// --- End of File ---
