//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SpriteRenderSystem.h"
#include "Sprite2DComponent.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "CameraNodeMan.h"
#include "Engine.h"

namespace Azul
{
	// (was GraphicsObject_Sprite::SetState/SetDataGPU/Draw/RestoreState)
	void SpriteRenderSystem::DrawSprite(Sprite2DComponent &s, Mat4 &world)
	{
		assert(s.pMesh);
		assert(s.pShader);
		assert(s.pTexture);

		// SetState
		s.pTexture->ActivateTexture();
		if (s.pTexture->HasAlpha())
		{
			Engine::GetInstance()->mBlendStateAlpha.Activate();
		}

		// SetDataGPU
		s.pShader->ActivateShader();
		s.pShader->ActivateCBV();

		Camera *pCam = CameraNodeMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		assert(pCam);

		Trans TransToOriginLowerLeft((float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Mat4 tmpMatrix = s.origMatrix * world * TransToOriginLowerLeft;

		s.pShader->TransferWorldViewProj(pCam, &tmpMatrix);
		s.pShader->TransferUVMatrix(&s.uvMatrix);
		s.pShader->TransferColorScale(&s.color);

		s.pMesh->ActivateMesh();

		// Draw
		s.pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}
}

// --- End of File ---
