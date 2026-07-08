#include "scene4.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "GameObjectMan.h"
#include "SystemMan.h"
#include "RotateSystem.h"
#include "RotateComponent.h"
#include "WorldMan.h"
#include "World.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"

#include "GraphicsObject_FlatTexture.h"
#include "GameObjectRigidBody.h"

namespace Azul
{
	bool Scene4::Load(Game& game)
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

		MeshNodeMan::Add(Mesh::Name::CUBE, "CubeMesh.m.proto.azul");

		ShaderObjectNodeMan::Add(ShaderObject::Name::FlatTexture);

		TexNodeMan::Add(TextureObject::Name::Ward, "Ward.t.proto.azul");

		// DEMO: a single cube whose spin is driven entirely by the ECS
		// RotateComponent + RotateSystem -- no Prefab, no per-object Update.
		{
			GraphicsObject* pGraphicsObject = new GraphicsObject_FlatTexture(
				Mesh::Name::CUBE,
				ShaderObject::Name::FlatTexture,
				TextureObject::Name::Ward);

			GameObjectRigidBody* pSpinner = new GameObjectRigidBody(pGraphicsObject);
			pSpinner->SetName("RotatingCube");
			pSpinner->SetTrans(0.0f, 0.0f, 0.0f);
			pSpinner->SetScale(100.0f);

			// Attach the behavior as data on the entity; RotateSystem does the rest.
			RotateComponent& rc = WorldMan::GetWorld().Add<RotateComponent>(pSpinner->GetEntity());
			rc.angle = 0.0f;
			rc.speed = 0.01f;

			GameObjectMan::Add(pSpinner, GameObjectMan::GetRoot());
		}

		return true;
	}

	void Scene4::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::Update();
		GameObjectMan::Update(tCurr, tDelta);
	}

	void Scene4::Unload(Game& game)
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
