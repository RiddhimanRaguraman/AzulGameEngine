#include "scene3.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "GameObjectMan.h"
#include "SystemMan.h"
#include "RotateSystem.h"
#include "WorldMan.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"

#include "GraphicsObject_FlatTexture.h"
#include "GameObjectRigidBody.h"
#include "GameObjectTerrain.h"

namespace Azul
{
	bool Scene3::Load(Game& game)
	{
		CameraNodeMan::Create();
		MeshNodeMan::Create();
		TexNodeMan::Create();
		ShaderObjectNodeMan::Create();
		GameObjectMan::Create();
		SystemMan::Add(new RotateSystem());

		CameraNodeMan::SetMoveSpeed(1.0f);

		{
			Camera* pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);

			pCam3D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam3D->setPerspective(35.0f,
				float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()),
				1.0f,
				100000.0f);

			Vec3 pos(0.0f, 150.0f, 450.0f);
			Vec3 tar(0.0f, 0.0f, 0.0f);
			Vec3 up = pos + Vec3(0.0f, 1.0f, 0.0f);
			pCam3D->SetHelper(up, tar, pos);

			pCam3D->updateCamera();
			CameraNodeMan::Add(Camera::Name::CAMERA_0, pCam3D);
			CameraNodeMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
		}

		{
			Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

			pCam2D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f,
				(float)pCam2D->getScreenWidth() / 2.0f,
				(float)-pCam2D->getScreenHeight() / 2.0f,
				(float)pCam2D->getScreenHeight() / 2.0f,
				1.0f,
				1000.0f);

			Vec3 pos(0.0f, 0.0f, 2.0f);
			Vec3 tar(0.0f, 0.0f, -1.0f);
			Vec3 up = pos + Vec3(0.0f, 1.0f, 0.0f);
			pCam2D->SetHelper(up, tar, pos);

			pCam2D->updateCamera();
			CameraNodeMan::Add(Camera::Name::CAMERA_1, pCam2D);
			CameraNodeMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);
		}

		MeshNodeMan::Add(Mesh::Name::Terrain, "Terrain.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SkyBox, "SkyBox.m.proto.azul");

		ShaderObjectNodeMan::Add(ShaderObject::Name::FlatTexture);

		TexNodeMan::Add(TextureObject::Name::Terrain, "Terrain.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::SkyBox, "SkyBox.t.proto.azul");

		{
			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(
				Mesh::Name::Terrain,
				ShaderObject::Name::FlatTexture,
				TextureObject::Name::Terrain);

			GameObjectTerrain* pTerrain = new GameObjectTerrain((GraphicsObject_FlatTexture*)pGraphicsObject, 200.0f, 200.0f, 20.0f);
			pTerrain->SetName("Terrain");
			pTerrain->SetTrans(0.0f, 0.0f, 0.0f);
			pTerrain->SetSize(800.0f, 800.0f);
			pTerrain->SetHeight(100.0f);
			pTerrain->SetUVRepeat(80.0f, 80.0f);
			GameObjectMan::Add(pTerrain, GameObjectMan::GetRoot());
		}

		{
			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(
				Mesh::Name::SkyBox,
				ShaderObject::Name::FlatTexture,
				TextureObject::Name::SkyBox);

			GameObjectRigidBody* pSkyBox = new GameObjectRigidBody(pGraphicsObject);
			pSkyBox->SetName("SkyBox");
			pSkyBox->SetTrans(0.0f, 0.0f, 0.0f);
			GameObjectMan::Add(pSkyBox, GameObjectMan::GetRoot());
		}

		return true;
	}

	void Scene3::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::Update();
		GameObjectMan::Update(tCurr, tDelta);
	}

	void Scene3::Unload(Game& game)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::SetMoveSpeed(0.1f);

		GameObjectMan::Destroy();
		WorldMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		TexNodeMan::Destroy();
		MeshNodeMan::Destroy();
		CameraNodeMan::Destroy();
	}
}
