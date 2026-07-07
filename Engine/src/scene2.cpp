#include "scene2.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "GameObjectMan.h"
#include "WorldMan.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"
#include "ImageMan.h"
#include "JointTableMan.h"
#include "GlyphMan.h"
#include "FontSprite.h"
#include "GraphicsObject_Sprite.h"
#include "SkelMan.h"
#include "ClipMan.h"
#include "AnimMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{
	static FontSprite* s_pFontSilly(nullptr);
	static FontSprite* s_pFontGangnam(nullptr);
	static FontSprite* s_pFontBlend(nullptr);
	static int s_blendLabelState(-1);

	bool Scene2::Load(Game& game)
	{
		//----------------------------------------------------------------------------------
		// Load Managers
		//----------------------------------------------------------------------------------
		CameraNodeMan::Create();
		MeshNodeMan::Create();
		TexNodeMan::Create();
		ShaderObjectNodeMan::Create();
		GameObjectMan::Create();
		ImageMan::Create();
		GlyphMan::Create();
		SkelMan::Create();
		ClipMan::Create();
		AnimMan::Create();
		JointTableMan::Create();
		HierarchyTableMan::Create();

		//----------------------------------------------------------------------------------
		// Camera Setup
		//----------------------------------------------------------------------------------
		
		// 3D perspective Camera
		{
			Camera* pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);

			pCam3D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam3D->setPerspective(35.0f,
				float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()),
				1.0f,
				100000.0f);

			Vec3 pos(0, 2, 12);
			Vec3 tar(0, 2, 0);
			Vec3 up = pos + Vec3(0, 1, 0);
			pCam3D->SetHelper(up, tar, pos);

			pCam3D->updateCamera();
			CameraNodeMan::Add(Camera::Name::CAMERA_0, pCam3D);
			CameraNodeMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
		}

		// 2D orthographic Camera
		{
			Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

			pCam2D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f,
				(float)pCam2D->getScreenWidth() / 2.0f,
				(float)-pCam2D->getScreenHeight() / 2.0f,
				(float)pCam2D->getScreenHeight() / 2.0f,
				1.0f,
				1000.0f);


			Vec3 pos(0, 0, 2);
			Vec3 tar(0, 0, -1);
			Vec3 up = pos + Vec3(0, 1, 0);
			pCam2D->SetHelper(up, tar, pos);

			pCam2D->updateCamera();
			CameraNodeMan::Add(Camera::Name::CAMERA_1, pCam2D);
			CameraNodeMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);
		}

		//----------------------------------------------------------------------------------
		// Mesh Setup
		//----------------------------------------------------------------------------------
		MeshNodeMan::Add(Mesh::Name::Mousey, "Mousey.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPRITE, "SpriteMesh.m.proto.azul");

		//----------------------------------------------------------------------------------
		// Joint Table Setup
		//----------------------------------------------------------------------------------
		JointTableMan::Add(JointTable::Name::Mousey, "Mousey.j.proto.azul");

		//----------------------------------------------------------------------------------
		// Hierarchy Table Setup
		//----------------------------------------------------------------------------------
		HierarchyTableMan::Add(HierarchyTable::Name::Mousey, "Mousey.h.proto.azul");

		//----------------------------------------------------------------------------------
		// Shaders Setup
		//----------------------------------------------------------------------------------
		ShaderObjectNodeMan::Add(ShaderObject::Name::Sprite);
		ShaderObjectNodeMan::Add(ShaderObject::Name::FlatTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::LightTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::SkinFlatTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::SkinLightTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::MixerACompute);
		ShaderObjectNodeMan::Add(ShaderObject::Name::MixerBCompute);
		ShaderObjectNodeMan::Add(ShaderObject::Name::MixerCCompute);
		ShaderObjectNodeMan::Add(ShaderObject::Name::WorldComputeA);
		ShaderObjectNodeMan::Add(ShaderObject::Name::WorldComputeC);


		//----------------------------------------------------------------------------------
		// Texture Setup 
		//----------------------------------------------------------------------------------
		TexNodeMan::Add(TextureObject::Name::Mousey, "Mousey.t.proto.azul");

		//----------------------------------------------------------------------------------
		// Font Animation
		//----------------------------------------------------------------------------------
		ImageMan::Add(Image::Name::GreenBird,
			TextureObject::Name::Test1,
			Rect(244.0f, 13.0f, 102.0f, 7.0f));

		TexNodeMan::Add(TextureObject::Name::FontAriel36, "Header.t.proto.azul");
		GlyphMan::Add(TextureObject::Name::FontAriel36, "HeaderMetrics.xml.proto.azul");

		GraphicsObject* pGraphicsObject(nullptr);
		Color color2(0.0f, 0.0f, 0.0f, 1.0f);
		FontSprite* pFontSprite(nullptr);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		pFontSprite = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestMessage, "Blending Animation", Glyph::Name::Arial36pt, 450, 850, color2);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		pFontSprite = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestMessage, "Press Spacebar to blend animation", Glyph::Name::Arial36pt, 350, 800, color2);


		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		pFontSprite = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestOneOff, "Press 1 to switch to Scene 1", Glyph::Name::Arial36pt, 20, 40, color2);

		//----------------------------------------------------------------------------------
		// Gameobject Animation
		//----------------------------------------------------------------------------------
		
		SkelMan::Add(Skel::Name::Mousey, "Mousey.s.proto.azul");

		Vec3 AnimLightColor(2.0f, 2.0f, 2.0f);
		Vec3 AnimLightPos(0.0f, 6.0f, 6.0f);

		AnimMan::Add(AnimMan::Name::Dance,
			"Mousey_SillyDancing.a.proto.azul",
			Skel::Name::Mousey,
			TextureObject::Name::Mousey,
			Mesh::Name::Mousey,
			AnimLightColor,
			AnimLightPos);
		AnimMan::SetPos(AnimMan::Name::Dance, -3.5f, 1.0f, 0.0f);
		AnimMan::SetUniformScale(AnimMan::Name::Dance, 1.5f);
		
		AnimMan::Add(AnimMan::Name::Gangnam,
			"Mousey_Gangnam.a.proto.azul",
			Skel::Name::Mousey,
			TextureObject::Name::Mousey,
			Mesh::Name::Mousey,
			AnimLightColor,
			AnimLightPos);
		AnimMan::SetPos(AnimMan::Name::Gangnam, 3.5f, 1.0f, 0.0f);
		AnimMan::SetUniformScale(AnimMan::Name::Gangnam, 1.5f);

		AnimMan::Add(AnimMan::Name::Blend,
					 "Mousey_Gangnam.a.proto.azul",
					 "Mousey_SillyDancing.a.proto.azul",
					 Skel::Name::Mousey,
					 TextureObject::Name::Mousey,
					 Mesh::Name::Mousey,
					 AnimLightColor,
					 AnimLightPos);
		AnimMan::SetPos(AnimMan::Name::Blend, 0.0f, 0.0f, 0.0f);
		AnimMan::SetUniformScale(AnimMan::Name::Blend, 3.5f);

		Color fontRed(1.0f, 0.0f, 0.0f, 1.0f);
		Color fontBlue(0.0f, 0.0f, 1.0f, 1.0f);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		s_pFontSilly = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(s_pFontSilly, GameObjectMan::GetRoot());
		s_pFontSilly->Set(FontSprite::Name::TestMessage, "Silly", Glyph::Name::Arial36pt, 150, 300, fontBlue );

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		s_pFontBlend = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(s_pFontBlend, GameObjectMan::GetRoot());
		s_pFontBlend->Set(FontSprite::Name::TestMessage, "Silly", Glyph::Name::Arial36pt, 550, 180, fontRed);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		s_pFontGangnam = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(s_pFontGangnam, GameObjectMan::GetRoot());
		s_pFontGangnam->Set(FontSprite::Name::TestMessage, "Gangnam", Glyph::Name::Arial36pt, 950, 300, fontRed);
		s_blendLabelState = -1;
		
		return true;
	}

	void Scene2::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::Update();

		AnimMan::BlendAnimation(tDelta);

		if (s_pFontBlend)
		{
			const float ts = AnimMan::GetBlendTs();
			int newState = 1;
			if (ts <= 0.01f)
			{
				newState = 0;
			}
			else if (ts >= 0.99f)
			{
				newState = 2;
			}

			if (newState != s_blendLabelState)
			{
				s_blendLabelState = newState;
				if (newState == 0)
				{
					s_pFontBlend->UpdateMessage("Gangnam");
					s_pFontBlend->pGraphicsObjectSprite->poColor->Set(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if (newState == 2)
				{
					s_pFontBlend->UpdateMessage("Silly");
					s_pFontBlend->pGraphicsObjectSprite->poColor->Set(0.0f, 0.0f, 1.0f, 1.0f);
				}
				else
				{
					s_pFontBlend->UpdateMessage("Blending");
					s_pFontBlend->pGraphicsObjectSprite->poColor->Set(0.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
		AnimMan::Update(tDelta);

		GameObjectMan::Update(tCurr);
	}

	void Scene2::Unload(Game& game)
	{
		AZUL_UNUSED_VAR(game);

		HierarchyTableMan::Destroy();
		JointTableMan::Destroy();
		AnimMan::Destroy();
		ClipMan::Destroy();
		SkelMan::Destroy();
		GlyphMan::Destroy();
		ImageMan::Destroy();
		GameObjectMan::Destroy();
		WorldMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		TexNodeMan::Destroy();
		MeshNodeMan::Destroy();
		CameraNodeMan::Destroy();

		s_pFontSilly = nullptr;
		s_pFontGangnam = nullptr;
		s_pFontBlend = nullptr;
		s_blendLabelState = -1;
	}
}
