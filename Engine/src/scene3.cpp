#include "scene3.h"

#include "Game.h"

#include "ShaderObjectNodeMan.h"
#include "MeshNodeMan.h"
#include "SystemMan.h"
#include "WorldMan.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"

#include "Renderable3D.h"
#include "RenderComponent.h"
#include "MathEngine.h"

namespace Azul
{
	bool Scene3::Load(Game& game)
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

		// Terrain -- FlatTexture renderable (RenderComponent holds the handles).
		{
			Entity terrain = Renderable3D::Add(MaterialKind::FlatTexture);

			RenderComponent& r = Renderable3D::GetRender(terrain);
			r.pMesh = MeshNodeMan::Find(Mesh::Name::Terrain);
			r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::FlatTexture);
			r.pTex = TexNodeMan::Find(TextureObject::Name::Terrain);
			r.uvMatrix = Scale(80.0f, 80.0f, 1.0f);   // was SetUVRepeat(80, 80)

			// Mesh authored at 200x200x20; place it at 800x800x100: scale = requested / authored.
			Renderable3D::SetScale(terrain, 800.0f / 200.0f, 100.0f / 20.0f, 800.0f / 200.0f);
			Renderable3D::SetTrans(terrain, 0.0f, 0.0f, 0.0f);
		}

		// SkyBox -- FlatTexture renderable.
		{
			Entity skyBox = Renderable3D::Add(MaterialKind::FlatTexture);
			Renderable3D::SetTrans(skyBox, 0.0f, 0.0f, 0.0f);

			RenderComponent& r = Renderable3D::GetRender(skyBox);
			r.pMesh = MeshNodeMan::Find(Mesh::Name::SkyBox);
			r.pShader = ShaderObjectNodeMan::Find(ShaderObject::Name::FlatTexture);
			r.pTex = TexNodeMan::Find(TextureObject::Name::SkyBox);
			r.layer = -1;   // background: draw before the terrain (alpha-blended skybox)
		}

		return true;
	}

	void Scene3::Update(Game& game, AnimTime tCurr, AnimTime tDelta)
	{
		AZUL_UNUSED_VAR(game);
		AZUL_UNUSED_VAR(tCurr);

		CameraNodeMan::Update();
		SystemMan::Run(WorldMan::GetWorld(), tDelta);
	}

	void Scene3::Unload(Game& game)
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
