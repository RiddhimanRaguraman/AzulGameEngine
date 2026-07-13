#include "scene4.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "MeshCubeColor.h"
#include "SystemMan.h"
#include "RotateComponent.h"
#include "WorldMan.h"
#include "World.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"

#include "Renderable3D.h"
#include "RenderComponent.h"

namespace Azul
{
	bool Scene4::Load(Game& game)
	{
		CameraNodeMan::Create();
		MeshNodeMan::Create();
		TexNodeMan::Create();
		ShaderObjectNodeMan::Create();
		SystemMan::Create();

		CameraNodeMan::SetMoveSpeed(1.0f);

		{
			Camera* pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);

			pCam3D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam3D->setPerspective(35.0f,
				float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()),
				1.0f,
				100000.0f);

			Vec3 pos(0.0f, 120.0f, 650.0f);
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
		// Procedural cube with a unique color per corner (the proto cube has no
		// vertex colors, so ColorByVertex renders it black).
		MeshNodeMan::Add(Mesh::Name::CUBE_COLOR, new MeshCubeColor(Mesh::Name::CUBE_COLOR));

		// Three material paths, all drawn by the P4.1 RenderSystem from the
		// component pool (each is a 3D MaterialKind).
		ShaderObjectNodeMan::Add(ShaderObject::Name::ColorByVertex);
		ShaderObjectNodeMan::Add(ShaderObject::Name::ConstColorLight);
		ShaderObjectNodeMan::Add(ShaderObject::Name::ConstColor);   // wireframe uses this + wire rasterizer

		// Three equally-spaced cubes, one per material, each spun by the ECS
		// RotateComponent + RotateSystem (no Prefab, no per-object Update).
		const float kSpacing = 220.0f;
		const float kScale = 70.0f;
		const float kSpin = 0.01f;

		// LEFT -- ColorByVertex on the procedural colored cube (unique color per
		// corner -> gradient faces). DATA-PATH: no GraphicsObject; RenderComponent
		// carries the mesh/shader handles, RenderSystem does the drawing.
		{
			Entity cube = Renderable3D::Add(MaterialKind::ColorByVertex);
			Renderable3D::SetTrans(cube, -kSpacing, 0.0f, 0.0f);
			Renderable3D::SetScale(cube, kScale*0.75f);

			RenderComponent& r = Renderable3D::GetRender(cube);
			r.pMesh = MeshNodeMan::Find(Mesh::Name::CUBE_COLOR);
			r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ColorByVertex);

			RotateComponent& rc = WorldMan::GetWorld().Add<RotateComponent>(cube);
			rc.angle = 0.0f;
			rc.speed = kSpin;
		}

		// MIDDLE -- ConstColorLight (constant body color + a light). DATA-PATH.
		{
			Entity cube = Renderable3D::Add(MaterialKind::ConstColorLight);
			Renderable3D::SetTrans(cube, 0.0f, 0.0f, 0.0f);
			Renderable3D::SetScale(cube, kScale);

			RenderComponent& r = Renderable3D::GetRender(cube);
			r.pMesh = MeshNodeMan::Find(Mesh::Name::CUBE);
			r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ConstColorLight);
			r.lightColor.set(1.0f, 1.0f, 1.0f);
			r.lightPos.set(150.0f, 200.0f, 300.0f);
			r.bodyColor.set(0.2f, 0.5f, 0.95f);   // blue-ish

			RotateComponent& rc = WorldMan::GetWorld().Add<RotateComponent>(cube);
			rc.angle = 0.0f;
			rc.speed = kSpin;
		}

		// RIGHT -- Wireframe (wire rasterizer + a constant color). DATA-PATH.
		{
			Entity cube = Renderable3D::Add(MaterialKind::Wireframe);
			Renderable3D::SetTrans(cube, kSpacing, 0.0f, 0.0f);
			Renderable3D::SetScale(cube, kScale);

			RenderComponent& r = Renderable3D::GetRender(cube);
			r.pMesh = MeshNodeMan::Find(Mesh::Name::CUBE);
			r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ConstColor);
			r.lightColor.set(0.1f, 1.0f, 0.3f);   // green wire color

			RotateComponent& rc = WorldMan::GetWorld().Add<RotateComponent>(cube);
			rc.angle = 0.0f;
			rc.speed = kSpin;
		}

		return true;
	}

	void Scene4::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);
		AZUL_UNUSED_VAR(tCurr);

		CameraNodeMan::Update();
		SystemMan::Run(WorldMan::GetWorld(), tDelta);
	}

	void Scene4::Unload(Game& game)
	{
		AZUL_UNUSED_VAR(game);

		CameraNodeMan::SetMoveSpeed(0.1f);

		SystemMan::Destroy();
		WorldMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		TexNodeMan::Destroy();
		MeshNodeMan::Destroy();
		CameraNodeMan::Destroy();
	}
}
