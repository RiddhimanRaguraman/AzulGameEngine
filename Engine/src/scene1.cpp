#include "scene1.h"

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
#include "SkelMan.h"
#include "ClipMan.h"
#include "AnimMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{
	bool Scene1::Load(Game& game)
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
		
		//---------------------------------------------------------------------------------------------------------
		// 3D perspective Camera
		//---------------------------------------------------------------------------------------------------------
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

		//---------------------------------------------------------------------------------------------------------
		// 2D orthographic Camera
		//---------------------------------------------------------------------------------------------------------
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
		MeshNodeMan::Add(Mesh::Name::Ward, "Ward.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPRITE, "SpriteMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Drax, "Drax.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Maw, "Maw.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Pirate, "Pirate.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Halo1, "Halo1.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Halo0, "Halo0.m.proto.azul");
		const unsigned int CROWNBOI_MESH_COUNT = 24;
		for (unsigned int i = 0; i < CROWNBOI_MESH_COUNT; i++)
		{
			const Mesh::Name meshName = (Mesh::Name)((int)Mesh::Name::Crownboi0 + (int)i);
			char meshFile[64]{ 0 };
			sprintf_s(meshFile, sizeof(meshFile), "Crownboi%u.m.proto.azul", i);
			MeshNodeMan::Add(meshName, meshFile);
		}
		//----------------------------------------------------------------------------------
		// Joint Table Setup
		//----------------------------------------------------------------------------------
		JointTableMan::Add(JointTable::Name::Ward, "Ward.j.proto.azul");
		JointTableMan::Add(JointTable::Name::Halo, "Halo.j.proto.azul");
		JointTableMan::Add(JointTable::Name::Crownboi, "Crownboi.j.proto.azul");
		JointTableMan::Add(JointTable::Name::Drax, "Drax.j.proto.azul");
		JointTableMan::Add(JointTable::Name::Maw, "Maw.j.proto.azul");
		JointTableMan::Add(JointTable::Name::Pirate, "Pirate.j.proto.azul");

		//----------------------------------------------------------------------------------
		// Hierarchy Table Setup
		//----------------------------------------------------------------------------------
		HierarchyTableMan::Add(HierarchyTable::Name::Ward, "Ward.h.proto.azul");
		HierarchyTableMan::Add(HierarchyTable::Name::Halo, "Halo.h.proto.azul");
		HierarchyTableMan::Add(HierarchyTable::Name::Crownboi, "Crownboi.h.proto.azul");
		HierarchyTableMan::Add(HierarchyTable::Name::Drax, "Drax.h.proto.azul");
		HierarchyTableMan::Add(HierarchyTable::Name::Maw, "Maw.h.proto.azul");
		HierarchyTableMan::Add(HierarchyTable::Name::Pirate, "Pirate.h.proto.azul");

		//----------------------------------------------------------------------------------
		// Skeleton Table Setup
		//----------------------------------------------------------------------------------
		SkelMan::Add(Skel::Name::Halo, "Halo.s.proto.azul");
		SkelMan::Add(Skel::Name::Ward, "Ward.s.proto.azul");
		SkelMan::Add(Skel::Name::Crownboi, "Crownboi.s.proto.azul");
		SkelMan::Add(Skel::Name::Drax, "Drax.s.proto.azul");
		SkelMan::Add(Skel::Name::Maw, "Maw.s.proto.azul");
		SkelMan::Add(Skel::Name::Pirate, "Pirate.s.proto.azul");

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
		TexNodeMan::Add(TextureObject::Name::Ward, "Ward.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Halo, "Halo.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Crownboi, "Crownboi.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Drax, "Drax.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Maw, "Maw.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Pirate, "Pirate.t.proto.azul");

		//----------------------------------------------------------------------------------
		// Font Animation
		//----------------------------------------------------------------------------------
		ImageMan::Add(Image::Name::WhiteBird,
					  TextureObject::Name::Test1,
					  Rect(139.0f, 131.0f, 84.0f, 97.0f));
		ImageMan::Add(Image::Name::ALLBirds,
					  TextureObject::Name::Test1,
					  Rect(0.0f, 0.0f, 377.0f, 234.0f));
		ImageMan::Add(Image::Name::GreenBird,
					  TextureObject::Name::Test1,
					  Rect(244.0f, 134.0f, 102.0f, 75.0f));

		TexNodeMan::Add(TextureObject::Name::FontAriel36, "Header.t.proto.azul");
		GlyphMan::Add(TextureObject::Name::FontAriel36, "HeaderMetrics.xml.proto.azul");

		Color color2(0.0f, 0.0f, 0.0f, 1.0f);
		FontSprite* pFontSprite(nullptr);

		pFontSprite = new FontSprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::GreenBird,
													Rect(100, 100, 100, 100));
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestMessage, "Multiple Animation", Glyph::Name::Arial36pt, 450, 850, color2);

		pFontSprite = new FontSprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::GreenBird,
													Rect(100, 100, 100, 100));
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestOneOff, "Press 2 to switch to Scene 2", Glyph::Name::Arial36pt, 20, 40, color2);

		//----------------------------------------------------------------------------------
		// Gameobject Animation
		//----------------------------------------------------------------------------------

		Vec3 AnimLightColor(1.8f, 1.8f, 1.8f);
		Vec3 AnimLightPos(0.0f, 6.0f, 6.0f);

		// Ward doing the Wave
		{
			AnimMan::Add(AnimMan::Name::Wave,
				"Ward_Wave.a.proto.azul",
				Skel::Name::Ward,
				TextureObject::Name::Ward,
				Mesh::Name::Ward,
				AnimLightColor,
				AnimLightPos);
			AnimMan::SetPos(AnimMan::Name::Wave, 0.0f, 3.0f, 0.0f);
			AnimMan::SetUniformScale(AnimMan::Name::Wave, 0.8f);

			pFontSprite = new FontSprite(Mesh::Name::SPRITE,
				ShaderObject::Name::Sprite,
				Image::GreenBird,
				Rect(100, 100, 100, 100));
				GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
			pFontSprite->Set(FontSprite::Name::TestOneOff, "Ward doing the Wave", Glyph::Name::Arial36pt, 450, 520, color2);
		}

		// Drax doing the bar Swing
		{
			AnimMan::Add(AnimMan::Name::Swing,
				"Drax_Swing.a.proto.azul",
				Skel::Name::Drax,
				TextureObject::Name::Drax,
				Mesh::Name::Drax,
				AnimLightColor,
				AnimLightPos);
			AnimMan::SetPos(AnimMan::Name::Swing, 0.0f, -0.5f, 0.0f);
			AnimMan::SetUniformScale(AnimMan::Name::Swing, 1.3f);

			pFontSprite = new FontSprite(Mesh::Name::SPRITE,
				ShaderObject::Name::Sprite,
				Image::GreenBird,
				Rect(100, 100, 100, 100));
				GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
			pFontSprite->Set(FontSprite::Name::TestOneOff, "Drax doing the bar Swing", Glyph::Name::Arial36pt, 420, 120, color2);
		}

		// Maw doing the Breakdance
		{
			AnimMan::Add(AnimMan::Name::Breakdance,
				"Maw_Breakdance.a.proto.azul",
				Skel::Name::Maw,
				TextureObject::Name::Maw,
				Mesh::Name::Maw,
				AnimLightColor,
				AnimLightPos);
			AnimMan::SetPos(AnimMan::Name::Breakdance, 3.0f, 3.0f, 0.0f);
			AnimMan::SetUniformScale(AnimMan::Name::Breakdance, 1.0f);

			pFontSprite = new FontSprite(Mesh::Name::SPRITE,
				ShaderObject::Name::Sprite,
				Image::GreenBird,
				Rect(100, 100, 100, 100));
				GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
			pFontSprite->Set(FontSprite::Name::TestOneOff, "Maw doing the Breakdance", Glyph::Name::Arial36pt, 800, 520, color2);
		}

		// Pirate doing the Salsa
		{
			AnimMan::Add(AnimMan::Name::Salsa,
				"Pirate_Salsa.a.proto.azul",
				Skel::Name::Pirate,
				TextureObject::Name::Pirate,
				Mesh::Name::Pirate,
				AnimLightColor,
				AnimLightPos);
			AnimMan::SetPos(AnimMan::Name::Salsa, -3.0f, 3.0f, 0.0f);
			AnimMan::SetUniformScale(AnimMan::Name::Salsa, 2.5f);

			pFontSprite = new FontSprite(Mesh::Name::SPRITE,
				ShaderObject::Name::Sprite,
				Image::GreenBird,
				Rect(100, 100, 100, 100));
				GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
			pFontSprite->Set(FontSprite::Name::TestOneOff, "Pirate doing the Salsa", Glyph::Name::Arial36pt, 60, 520, color2);
		}

		// Halo doing the shuffle
		{
			Mesh::Name haloMeshes[2]
			{
				Mesh::Name::Halo0,
				Mesh::Name::Halo1
			};

			AnimMan::Add(AnimMan::Name::Shuffle,
				"Halo_Shuffling.a.proto.azul",
				Skel::Name::Halo,
				TextureObject::Name::Halo,
				haloMeshes,
				2,
				AnimLightColor,
				AnimLightPos);
			AnimMan::SetPos(AnimMan::Name::Shuffle, -3.0f, -0.5f, 0.0f);
			AnimMan::SetUniformScale(AnimMan::Name::Shuffle, 1.5f);

			pFontSprite = new FontSprite(Mesh::Name::SPRITE,
				ShaderObject::Name::Sprite,
				Image::GreenBird,
				Rect(100, 100, 100, 100));
				GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
			pFontSprite->Set(FontSprite::Name::TestOneOff, "Halo doing the shuffle", Glyph::Name::Arial36pt, 60, 120, color2);
		}

		// Crownboi doing Rumba
		Mesh::Name crownMeshes[CROWNBOI_MESH_COUNT];
		for (unsigned int i = 0; i < CROWNBOI_MESH_COUNT; i++)
		{
			crownMeshes[i] = (Mesh::Name)((int)Mesh::Name::Crownboi0 + (int)i);
		}

		AnimMan::Add(AnimMan::Name::Rumba,
			"Crownboi_Rumba.a.proto.azul",
			Skel::Name::Crownboi,
			TextureObject::Name::Crownboi,
			crownMeshes,
			CROWNBOI_MESH_COUNT,
			AnimLightColor,
			AnimLightPos);
		AnimMan::SetPos(AnimMan::Name::Rumba, 3.0f, -0.5f, 0.0f);
		AnimMan::SetUniformScale(AnimMan::Name::Rumba, 1.3f);

		pFontSprite = new FontSprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,
			Rect(100, 100, 100, 100));
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());
		pFontSprite->Set(FontSprite::Name::TestOneOff, "Crownboi doing Rumba", Glyph::Name::Arial36pt, 820, 120, color2);

		return true;
	}

	void Scene1::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::Update();

		GameObjectMan::Update(tCurr, tDelta);
	}

	void Scene1::Unload(Game& game)
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
	}
}
