//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "GameObjectMan.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"
#include "AnimTimer.h"
#include "ImageMan.h"
#include "JointTableMan.h"
#include "Prefab_Pivot.h"
#include "GlyphMan.h"
#include "FontSprite.h"
#include "SkelMan.h"
#include "ClipMan.h"
#include "Anim.h"
#include "TimerController.h"
#include "AnimController.h"
#include "AnimMan.h"
#include "JointTableProto.h"
 
namespace Azul
{
 
  FontSprite* pFontSprite1;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------

	Game::Game(const char *const pName, int width, int height)
		: Engine(pName, width, height)
	{
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	//  Game::~Game()
	//		Game Engine destructor
	//-----------------------------------------------------------------------------
	Game::~Game()
	{
		Trace::out("\n");
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
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

		// --------------------------------- 
		//  Camera - Setup 
		// --------------------------------- 

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 3D perspective Camera
		//---------------------------------------------------------------------------------------------------------
		Camera* pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);

		pCam3D->setViewport(0, 0, this->mWindowWidth, this->mWindowHeight);
		pCam3D->setPerspective(35.0f,
			float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()),
			1.0f,
			100000.0f);

		Vec3 pos(-1, 12, 0);
		Vec3 tar(-1, 0, 0);
		Vec3 up = pos + Vec3(1, 0, 0);
		pCam3D->SetHelper(up, tar, pos);

		pCam3D->updateCamera();
		CameraNodeMan::Add(Camera::Name::CAMERA_0, pCam3D);
		CameraNodeMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 2D orthographic Camera
		//---------------------------------------------------------------------------------------------------------
		Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

		pCam2D->setViewport(0, 0, this->mWindowWidth, this->mWindowHeight);
		pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f,
			(float)pCam2D->getScreenWidth() / 2.0f,
			(float)-pCam2D->getScreenHeight() / 2.0f,
			(float)pCam2D->getScreenHeight() / 2.0f,
			1.0f,
			1000.0f);
		pCam2D->setOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f),
			Vec3(0.0f, 0.0f, -1.0f),
			Vec3(0.0f, 0.0f, 2.0f));
		pCam2D->updateCamera();
		CameraNodeMan::Add(Camera::Name::CAMERA_1, pCam2D);
		CameraNodeMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);
		
		// --------------------------------
		//  Mesh
		// --------------------------------

		Mesh* poSpriteMesh = new MeshProto("SpriteMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPRITE, poSpriteMesh);

		Mesh* pChickenBotMesh = new MeshProto("ChickenBot.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::ChickenBot, pChickenBotMesh);

		Mesh* pMouseyMesh = new MeshProto("Mousey.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::Mousey, pMouseyMesh);

		Mesh* pMeshR2D2 = new MeshProto("R2D2.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::R2D2, pMeshR2D2);

		TextureObject* pTexR2D2_BASE = new TextureObject("R2D2_Base.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::R2D2_BASE, pTexR2D2_BASE);

		Mesh* pMeshcrate = new MeshProto("crate.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CRATE, pMeshcrate);

		TextureObject* pTexcrate = new TextureObject("crate.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Crate, pTexcrate);

		Mesh* pMeshfrig = new MeshProto("space_frigate.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPACE_FRIGATE, pMeshfrig);

		TextureObject* pTexFrig = new TextureObject("space_frigate.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::SpaceFrigate, pTexFrig);

		// --------------------------------
		//   Joint
		// --------------------------------

		JointTable* pJointTableChicken = JointTableMan::Add(JointTable::Name::ChickenBot, "ChickenBot.j.proto.azul");
		JointTable* pJointTableMouse = JointTableMan::Add(JointTable::Name::Mousey, "Mousey.j.proto.azul");

		// --------------------------------
		//  Shader
		// --------------------------------

		ShaderObjectNodeMan::Add(ShaderObject::Name::Sprite);
		ShaderObjectNodeMan::Add(ShaderObject::Name::FlatTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::LightTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::SkinFlatTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::SkinLightTexture);
		ShaderObjectNodeMan::Add(ShaderObject::Name::BasicCompute);
		ShaderObjectNodeMan::Add(ShaderObject::Name::MixerCompute);

		// --------------------------------
		//  Texture
		// --------------------------------

		TexNodeMan::Add("TEST_PNG_RGB.t.proto.azul", TextureObject::Name::Test0);
		TexNodeMan::Add("TEST_PNG_RGBA.t.proto.azul", TextureObject::Name::Test1);
		TexNodeMan::Add("TEST_TGA_BGR.t.proto.azul", TextureObject::Name::Test2);
		TexNodeMan::Add("TEST_TGA_BGRA.t.proto.azul", TextureObject::Name::Test3);
		//TexNodeMan::Add("ChickenBot.t.proto.azul",TextureObject::Name::ChickenBot);
		TexNodeMan::Add("Mousey.t.proto.azul", TextureObject::Name::Mousey);

	/*	TextureObject* pTexDogBot = new TextureObject("DogBot.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::DogBot, pTexDogBot);

		TextureObject* pTexSpiderBot = new TextureObject("SpiderBot.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::SpiderBot, pTexSpiderBot);*/
		// -----------------------------------
		//  Image
		// -----------------------------------

		ImageMan::Add(Image::Name::WhiteBird,
			TextureObject::Name::Test1,
			Rect(139.0f, 131.0f, 84.0f, 97.0f));
		ImageMan::Add(Image::Name::ALLBirds,
			TextureObject::Name::Test1,
			Rect(0.0f, 0.0f, 377.0f, 234.0f));
		ImageMan::Add(Image::Name::GreenBird,
			TextureObject::Name::Test1,
			Rect(244.0f, 134.0f, 102.0f, 75.0f));

		// ---------------------------------------------
		//  Font - load xml
		// ---------------------------------------------
		TexNodeMan::Add("FontArial36.t.proto.azul", TextureObject::Name::FontAriel36);
		GlyphMan::Add("MetricsArial36.xml.proto.azul", TextureObject::Name::FontAriel36);


		// ---------------------------------------------
		//  GraphicsObject -- needs model + shader
		// ---------------------------------------------
		GraphicsObject* pGraphicsObject(nullptr);
		GameObjectRigidBody* pGameObj(nullptr);

		Vec3 LightColor(1.0f, 1.0f, 1.0f);
		Vec3 LightPos(1.0f, 1.0f, 100.0f);
		Vec3 color(Azul::Colors::Blue);
		

		pGraphicsObject = new GraphicsObject_Wireframe(Mesh::Name::R2D2,
			ShaderObject::Name::LightTexture,
			color);

		pGameObj = new GameObjectRigidBody(pGraphicsObject);
		pGameObj->SetName("R2D2");
		pGameObj->SetScale(80.0f);
		pGameObj->SetTrans(0.0f, 0.0f, 3.5f);
		pGameObj->SetRotY(1.57f);
		pGameObj->SetPrefab(new Prefab_Pivot());
		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

		pGraphicsObject = new GraphicsObject_LightTexture(Mesh::Name::R2D2,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::R2D2_BASE,
			LightColor, LightPos);

		pGameObj = new GameObjectRigidBody(pGraphicsObject);
		pGameObj->SetName("R2D2");
		pGameObj->SetScale(80.0f);
		pGameObj->SetTrans(0.0f, 0.0f, -3.8f);
		pGameObj->SetRotY(1.57f);
		pGameObj->SetPrefab(new Prefab_Pivot());
		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

		pGraphicsObject = new GraphicsObject_LightTexture(Mesh::Name::SPACE_FRIGATE,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::SpaceFrigate,
			LightColor, LightPos);

		pGameObj = new GameObjectRigidBody(pGraphicsObject);
		pGameObj->SetName("Fig");
		pGameObj->SetScale(0.04f);
		pGameObj->SetTrans(-3.0f, 0.0f, 3.8f);
		pGameObj->SetRotY(1.57f);
		pGameObj->SetRotZ(-1.57f);
		pGameObj->SetPrefab(new Prefab_Pivot());
		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

		pGraphicsObject = new GraphicsObject_LightTexture(Mesh::Name::CRATE,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::Crate,
			LightColor, LightPos);

		pGameObj = new GameObjectRigidBody(pGraphicsObject);
		pGameObj->SetName("crate");
		pGameObj->SetScale(0.8f);
		pGameObj->SetTrans(-3.0f, 0.0f, -3.8f);
		pGameObj->SetRotY(1.57f);
		pGameObj->SetPrefab(new Prefab_Pivot());
		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::GreenBird,
													Rect(500, 100, 100, 100));
		
		GameObjectSprite* pSprite1 = new GameObjectSprite(pGraphicsObject);
		GameObjectMan::Add(pSprite1, GameObjectMan::GetRoot());

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::WhiteBird,
													Rect(300, 100, 100, 100));

		GameObjectSprite* pSprite2 = new GameObjectSprite(pGraphicsObject);
		GameObjectMan::Add(pSprite2, GameObjectMan::GetRoot());

		// FontSprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSprite1 = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite1, GameObjectMan::GetRoot());

		Color color2(0.0f, 0, 1.0f, 1.0f);
		pFontSprite1->Set(FontSprite::Name::TestMessage, "ABCD", Glyph::Name::Arial36pt, 300, 550, color2);

		FontSprite* pFontSprite(nullptr);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSprite = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());

		pFontSprite->Set(FontSprite::Name::TestMessage, "Dance", Glyph::Name::Arial36pt, 250, 250, color2);

		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSprite = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSprite, GameObjectMan::GetRoot());

		pFontSprite->Set(FontSprite::Name::TestMessage, "Walk", Glyph::Name::Arial36pt, 450, 250, color2);

		// ---------------------------------
		// Create Animation
		// ---------------------------------

		//// load skeleton
		SkelMan::Add("ChickenBot.s.proto.azul", Skel::Name::ChickenBot);
		SkelMan::Add("Mousey.s.proto.azul", Skel::Name::Mousey);
		//SkelMan::Add("DogBot.s.proto.azul", Skel::Name::DogBot);
		//SkelMan::Add("SpiderBot.s.proto.azul", Skel::Name::SpiderBot);

		AnimTime delta = 0.5f * AnimTime(AnimTime::Duration::FILM_24_FRAME);

		/*ClipMan::Add("ChickenBot_HitBack.a.proto.azul", Clip::Name::HitBack_ChickenBot, Skel::Name::ChickenBot);
		ClipMan::Add("ChickenBot_ShotUp.a.proto.azul", Clip::Name::ShotUp_ChickenBot, Skel::Name::ChickenBot);*/

		AnimMan::Add(AnimMan::Name::Dance, "Mousey_SillyDancing.a.proto.azul", delta, Skel::Name::Mousey, TextureObject::Name::Mousey, Mesh::Name::Mousey, pJointTableMouse);
		AnimMan::SetPos(AnimMan::Name::Dance, -1.2f, 0.0f, -1.2f);
		AnimMan::SetUniformScale(AnimMan::Name::Dance, 2.5f);
		AnimMan::SetPivotTotalRot(AnimMan::Name::Dance, Rot3::ZYX, -MATH_PI / 2, 0.0f, -MATH_PI / 2);


		AnimMan::Add(AnimMan::Name::Run, "ChickenBot_Walk.a.proto.azul", delta, Skel::Name::ChickenBot, TextureObject::Name::ChickenBot, Mesh::Name::ChickenBot, pJointTableChicken);
		AnimMan::SetPos(AnimMan::Name::Run, -1.2f, 0.0f, 1.2f);
		AnimMan::SetUniformScale(AnimMan::Name::Run, 60.0f);
		AnimMan::SetPivotTotalRot(AnimMan::Name::Run, Rot3::ZYX, -MATH_PI / 2, 0.0f, -MATH_PI / 2);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------

    void Game::Update()
    {

		this->intervalTimer.Toc();
		this->intervalTimer.Tic();

		CameraNodeMan::Update();
		// ------------------------------------
		// Animate Me
		// ------------------------------------
	
		/*static bool useHitBackShotUp = false;
		
		short tState = GetAsyncKeyState('T');
		short rState = GetAsyncKeyState('R');
		
		
		if (tState & 0x8000)
		{
			if (!useHitBackShotUp)
			{
				useHitBackShotUp = true;

				AnimController* pWalkCtrl = AnimMan::Find(AnimMan::Name::Walk);
				if (pWalkCtrl)
				{
					pWalkCtrl->SetClip(Clip::Name::HitBack_ChickenBot);
				}

				AnimController* pRunCtrl = AnimMan::Find(AnimMan::Name::Run);
				if (pRunCtrl)
				{
					pRunCtrl->SetClip(Clip::Name::ShotUp_ChickenBot);
				}
			}
		}
		else if (rState & 0x8000)
		{
			if (useHitBackShotUp)
			{
				useHitBackShotUp = false;

				AnimController* pWalkCtrl = AnimMan::Find(AnimMan::Name::Walk);
				if (pWalkCtrl)
				{
					pWalkCtrl->SetClip(Clip::Name::Walk_ChickenBot);
				}

				AnimController* pRunCtrl = AnimMan::Find(AnimMan::Name::Run);
				if (pRunCtrl)
				{
					pRunCtrl->SetClip(Clip::Name::Run_ChickenBot);
				}
			}
		}*/

        AnimMan::Update();

		static int count = 0;
		count++;
		char buff[20];
		memset(buff, 0x0, 20);
		sprintf_s(buff, 20, "Test: %d", count);
		pFontSprite1->UpdateMessage(buff);

		// ------------------------------------
		// Update GameObjects
		// ------------------------------------
		GameObjectMan::Update(this->globalTimer.Toc());

	}


	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		this->SetDefaultTargetMode();

		GameObjectMan::Draw();
	}

	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnloadContent()
	{
		JointTableMan::Destroy();
		AnimMan::Destroy();
		ClipMan::Destroy();
		SkelMan::Destroy();
		GlyphMan::Destroy();
		ImageMan::Destroy();
		GameObjectMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		TexNodeMan::Destroy();
		MeshNodeMan::Destroy();
		CameraNodeMan::Destroy();

	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	// 
	// Clear the color and depth buffers.
	//------------------------------------------------------------------
	void Game::ClearDepthStencilBuffer()
	{
#ifdef _DEBUG
		this->mStateRenderTargetView.Clear(Azul::Colors::LightGray);
		this->mDepthStencilView.Clear(1.0f, 0);
#else
		this->mStateRenderTargetView.Clear(Azul::Colors::Wheat);
		this->mDepthStencilView.Clear(1.0f, 0);
#endif

	}

}

// --- End of File ---
