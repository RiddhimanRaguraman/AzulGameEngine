//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "RenderSystem.h"
#include "RenderComponent.h"
#include "GraphicsObject.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_ConstColorLight.h"
#include "GraphicsObject_Wireframe.h"
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
		assert(r.pGraphicsObject);

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

		default:
			// Not-yet-migrated materials (SkinLightTexture): bridge through
			// the GraphicsObject.
			r.pGraphicsObject->Render();
			break;
		}
	}

	// (was GraphicsObject_ColorByVertex::SetState/SetDataGPU/Draw/RestoreState)
	void RenderSystem::privDrawColorByVertex(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pGraphicsObject->GetMesh();
		ShaderObject *pShader = r.pGraphicsObject->GetShader();

		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);

		pMesh->RenderIndexBuffer();
	}

	// (was GraphicsObject_FlatTexture::SetState/SetDataGPU/Draw/RestoreState)
	void RenderSystem::privDrawFlatTexture(RenderComponent &r, Mat4 &world)
	{
		GraphicsObject_FlatTexture *pGO = (GraphicsObject_FlatTexture *)r.pGraphicsObject;
		Mesh *pMesh = pGO->GetMesh();
		ShaderObject *pShader = pGO->GetShader();

		// SetState
		pGO->pTex->ActivateTexture();
		if (pGO->pTex->HasAlpha())
		{
			Engine::GetInstance()->mBlendStateAlpha.Activate();
		}

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferUVMatrix(&pGO->uvMatrix);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}

	// (was GraphicsObject_ConstColorLight::SetState/SetDataGPU/Draw/RestoreState)
	void RenderSystem::privDrawConstColorLight(RenderComponent &r, Mat4 &world)
	{
		GraphicsObject_ConstColorLight *pGO = (GraphicsObject_ConstColorLight *)r.pGraphicsObject;
		Mesh *pMesh = pGO->GetMesh();
		ShaderObject *pShader = pGO->GetShader();

		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferPos(pGO->poLightPos);
		pShader->TransferColor(pGO->poLightColor);

		Color col(*pGO->poBodyColor);
		pShader->TransferBodyColor(&col);

		pMesh->RenderIndexBuffer();
	}

	// (was GraphicsObject_Wireframe::SetState/SetDataGPU/Draw/RestoreState)
	void RenderSystem::privDrawWireframe(RenderComponent &r, Mat4 &world)
	{
		GraphicsObject_Wireframe *pGO = (GraphicsObject_Wireframe *)r.pGraphicsObject;
		Mesh *pMesh = pGO->GetMesh();
		ShaderObject *pShader = pGO->GetShader();

		// SetState
		Engine::GetInstance()->mStateRasterizerWireNoCull.Activate();

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->ActivateShader();
		pShader->ActivateCBV();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferColor(pGO->poLightColor);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mStateRasterizerSolidCull.Activate();
	}
}

// --- End of File ---
