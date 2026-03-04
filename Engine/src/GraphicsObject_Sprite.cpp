//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_Sprite.h"
#include "CameraNodeMan.h"
#include "GameMan.h"
#include "ImageMan.h"

namespace Azul
{

	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	
	GraphicsObject_Sprite::GraphicsObject_Sprite(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		Image::Name imageName,
		Rect rect)
		: GraphicsObject(meshName, shaderName)
	{

		this->poColor = new Color(1.0f, 1.0f, 1.0f, 1.0f);
		assert(poColor);

		this->pImage = ImageMan::Find(imageName);
		assert(pImage);

		this->pTexture = pImage->pText;

		this->origWidth = rect.width;
		this->origHeight = rect.height;

		this->origPosX = rect.x;
		this->origPosY = rect.y;

		// Create the UV matrix
		this->poMatrix_uv = new Mat4();

		float w = pImage->imageRect.width / (float)pImage->pText->width;
		float h = pImage->imageRect.height / (float)pImage->pText->height;
		float u = pImage->imageRect.x / (float)pImage->pText->width;
		float v = pImage->imageRect.y / (float)pImage->pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;

		// Create the orig Matrix 
		// The initial sprite setup, after this point standard world will adjust
		this->poMatrix_orig = new Mat4();

		Scale Scale(this->origWidth, this->origHeight, 1.0f);
		*this->poMatrix_orig = Scale;
	}

	// w1(7)
	GraphicsObject_Sprite::~GraphicsObject_Sprite()
	{
		delete this->poMatrix_uv;
		delete this->poMatrix_orig;
		delete this->poColor;
	}


	void GraphicsObject_Sprite::SetTexture(TextureObject* pText)
	{
		assert(pText);
		this->pTexture = pText;
	}


	void GraphicsObject_Sprite::SetScreenRect(Rect& rect)
	{
		this->origWidth = rect.width;
		this->origHeight = rect.height;

		this->origPosX = rect.x;
		this->origPosY = rect.y;

		Scale Scale(this->origWidth, this->origHeight, 1.0f);
		*this->poMatrix_orig = Scale;
	}

	void GraphicsObject_Sprite::SetImage(Rect& imageRect, TextureObject* pText)
	{
		assert(pText);

		float w = imageRect.width / (float)pText->width;
		float h = imageRect.height / (float)pText->height;
		float u = imageRect.x / (float)pText->width;
		float v = imageRect.y / (float)pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;
	}

	void GraphicsObject_Sprite::SetImage(Image::Name imageName)
	{
		this->pImage = ImageMan::Find(imageName);
		assert(pImage);

		float w = pImage->imageRect.width / (float)pImage->pText->width;
		float h = pImage->imageRect.height / (float)pImage->pText->height;
		float u = pImage->imageRect.x / (float)pImage->pText->width;
		float v = pImage->imageRect.y / (float)pImage->pText->height;
		Scale ScaleUV(w, h, 1.0f);
		Trans TransUV(u, v, 0.0f);
		*this->poMatrix_uv = ScaleUV * TransUV;
	}


	void GraphicsObject_Sprite::SetState()
	{
		// Future - settings to directX
		// say make it wireframe or change culling mode
		this->pTexture->ActivateTexture();

		if (this->pTexture->HasAlpha())
		{
			GameMan::GetGame()->mBlendStateAlpha.Activate();
		}
	}

	void GraphicsObject_Sprite::SetDataGPU()
	{
		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		// World matrix ----------------------------
		// w1(7)
		Camera* pCam = CameraNodeMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		assert(pCam);

		Trans TransToOriginLowerLeft((float)-pCam->getScreenWidth() / 2.0f, (float)-pCam->getScreenHeight() / 2.0f, 0.0f);
		Mat4 tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;

		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D), &tmpMatrix);

		pShaderObj->TransferUVMatrix(this->poMatrix_uv);
		pShaderObj->TransferColorScale(this->poColor);

		pMesh->ActivateMesh();
	}

	void GraphicsObject_Sprite::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_Sprite::RestoreState()
	{
		GameMan::GetGame()->mBlendStateOff.Activate();
	}
}

// --- End of File ---
