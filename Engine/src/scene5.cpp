#include "scene5.h"

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
	bool Scene5::Load(Game& game)
	{
		CameraNodeMan::Create();
		MeshNodeMan::Create();
		TexNodeMan::Create();
		ShaderObjectNodeMan::Create();
		SystemMan::Create();

		CameraNodeMan::SetMoveSpeed(2.0f);

		{
			Camera* pCam3D = new Camera(Camera::Type::PERSPECTIVE_3D);

			pCam3D->setViewport(0, 0, game.mWindowWidth, game.mWindowHeight);
			pCam3D->setPerspective(45.0f,
				float(pCam3D->getScreenWidth()) / float(pCam3D->getScreenHeight()),
				1.0f,
				100000.0f);

			Vec3 pos(350.0f, 350.0f, 950.0f);
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

		// One shared mesh + one shared material for every cube (so they batch and,
		// later, instance). The procedural colored cube gives per-corner gradients.
		MeshNodeMan::Add(Mesh::Name::CUBE_COLOR, new MeshCubeColor(Mesh::Name::CUBE_COLOR));
		ShaderObjectNodeMan::Add(ShaderObject::Name::ColorByVertex);

		Mesh* pCubeMesh = MeshNodeMan::Find(Mesh::Name::CUBE_COLOR);
		ShaderObject* pCubeShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ColorByVertex);
		assert(pCubeMesh);
		assert(pCubeShader);

		// 10x10x10 = 1000 cubes, centered on the origin.
		const int   kGrid = 10;
		const float kSpacing = 45.0f;
		const float kScale = 15.0f;
		const float kSpin = 0.01f;
		const float half = (float)(kGrid - 1) * kSpacing * 0.5f;

		World& world = WorldMan::GetWorld();

		for (int ix = 0; ix < kGrid; ix++)
		{
			for (int iy = 0; iy < kGrid; iy++)
			{
				for (int iz = 0; iz < kGrid; iz++)
				{
					Entity cube = Renderable3D::Add(MaterialKind::ColorByVertex);
					Renderable3D::SetTrans(cube,
						(float)ix * kSpacing - half,
						(float)iy * kSpacing - half,
						(float)iz * kSpacing - half);
					Renderable3D::SetScale(cube, kScale);

					RenderComponent& r = Renderable3D::GetRender(cube);
					r.pMesh = pCubeMesh;
					r.pShader = pCubeShader;

					RotateComponent& rc = world.Add<RotateComponent>(cube);
					rc.angle = (float)(ix + iy + iz) * 0.15f;   // varied phase
					rc.speed = kSpin;
				}
			}
		}

		return true;
	}

	void Scene5::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);
		AZUL_UNUSED_VAR(tCurr);

		CameraNodeMan::Update();
		SystemMan::Run(WorldMan::GetWorld(), tDelta);
	}

	void Scene5::Unload(Game& game)
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
