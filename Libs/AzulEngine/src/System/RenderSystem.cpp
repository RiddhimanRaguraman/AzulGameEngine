//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "RenderSystem.h"
#include "RenderComponent.h"
#include "ComputeBlend.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "Color.h"
#include "CameraNodeMan.h"
#include "Engine.h"

namespace Azul
{
	void RenderSystem::DrawObject(RenderComponent &r, Mat4 &world)
	{
		switch (r.kind)
		{
		case MaterialKind::ColorByVertex:
			privDrawColorByVertex(r, world);
			break;

		case MaterialKind::FlatTexture:
			privDrawFlatTexture(r, world);
			break;

		case MaterialKind::ConstColorLight:
			privDrawConstColorLight(r, world);
			break;

		case MaterialKind::Wireframe:
			privDrawWireframe(r, world);
			break;

		case MaterialKind::SkinLightTexture:
			privDrawSkinLightTexture(r, world);
			break;

		default:
			// DrawObject is only called for 3D kinds (all have a branch above);
			// Null/Sprite never reach here.
			assert(false);
			break;
		}
	}

	// (was GraphicsObject_ColorByVertex::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: reads mesh/shader from RenderComponent (no GraphicsObject).
	void RenderSystem::privDrawColorByVertex(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);

		pMesh->RenderIndexBuffer();
	}

	// (was GraphicsObject_FlatTexture::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: mesh/shader/tex/uvMatrix from RenderComponent.
	void RenderSystem::privDrawFlatTexture(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);
		assert(r.pTex);

		// SetState
		r.pTex->ActivateTexture();
		if (r.pTex->HasAlpha())
		{
			Engine::GetInstance()->mBlendStateAlpha.Activate();
		}

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferUVMatrix(&r.uvMatrix);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}

	// (was GraphicsObject_ConstColorLight::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: params from RenderComponent.
	void RenderSystem::privDrawConstColorLight(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferPos(&r.lightPos);
		pShader->TransferColor(&r.lightColor);

		Color col(r.bodyColor);
		pShader->TransferBodyColor(&col);

		pMesh->RenderIndexBuffer();
	}

	// (was GraphicsObject_Wireframe::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: color from RenderComponent.lightColor.
	void RenderSystem::privDrawWireframe(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		// SetState
		Engine::GetInstance()->mStateRasterizerWireNoCull.Activate();

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferColor(&r.lightColor);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mStateRasterizerSolidCull.Activate();
	}

	// (was GraphicsObject_SkinLightTexture::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: mesh/shader/tex/light/ComputeBlend from RenderComponent.
	// Consumes the SkinningSystem output: BindWorldBoneArray binds the bone-world
	// SRV that ComputeBlend::Execute produced during Update (runs before Draw).
	void RenderSystem::privDrawSkinLightTexture(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);
		assert(r.pTex);
		assert(r.pComputeBlend);

		// SetState
		r.pTex->ActivateTexture();
		Engine::GetInstance()->mStateRasterizerSolidCull.Activate();

		// SetDataGPU
		pMesh->ActivateMesh();
		pMesh->ActivateSRVBuffers();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferPos(&r.lightPos);
		pShader->TransferColor(&r.lightColor);
		r.pComputeBlend->BindWorldBoneArray();

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}
}

// --- End of File ---
