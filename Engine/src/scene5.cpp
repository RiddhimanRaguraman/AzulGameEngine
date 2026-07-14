#include "scene5.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
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

		// One shared mesh + material for every cube. The cube geometry is loaded
		// from disk (protobuf serialization); it ships without vertex colors, so we
		// set the per-vertex colors here on the client side (ColorByVertex reads
		// them). Every cube is a separate ECS entity drawn per-object -- no
		// instancing -- to show the ECS handles a huge object count.
		MeshNodeMan::Add(Mesh::Name::CUBE, "CubeMesh.m.proto.azul");
		ShaderObjectNodeMan::Add(ShaderObject::Name::ColorByVertex);

		Mesh* pCubeMesh = MeshNodeMan::Find(Mesh::Name::CUBE);
		ShaderObject* pCubeShader = ShaderObjectNodeMan::Find(ShaderObject::Name::ColorByVertex);
		assert(pCubeMesh);
		assert(pCubeShader);

		// Client-chosen per-vertex colors (RGBA, one per mesh vertex). A vivid
		// palette cycled over the cube's vertices -> each vertex a different color.
		{
			const float palette[8][4] =
			{
				{ 1.0f, 0.0f, 0.0f, 1.0f },   // red
				{ 1.0f, 1.0f, 0.0f, 1.0f },   // yellow
				{ 0.0f, 1.0f, 0.0f, 1.0f },   // green
				{ 0.0f, 1.0f, 1.0f, 1.0f },   // cyan
				{ 0.0f, 0.0f, 1.0f, 1.0f },   // blue
				{ 1.0f, 0.0f, 1.0f, 1.0f },   // magenta
				{ 1.0f, 1.0f, 1.0f, 1.0f },   // white
				{ 1.0f, 0.5f, 0.0f, 1.0f },   // orange
			};

			const unsigned int numVerts = pCubeMesh->numVerts;
			float* pColors = new float[numVerts * 4];
			for (unsigned int v = 0; v < numVerts; v++)
			{
				const float* c = palette[v % 8];
				pColors[v * 4 + 0] = c[0];
				pColors[v * 4 + 1] = c[1];
				pColors[v * 4 + 2] = c[2];
				pColors[v * 4 + 3] = c[3];
			}
			pCubeMesh->SetVertexColors(pColors, numVerts * 4 * (unsigned int)sizeof(float), 4 * (unsigned int)sizeof(float));
			delete[] pColors;
		}

		// kGrid^3 cubes, centered on the origin.
		const int   kGrid = 30;
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
