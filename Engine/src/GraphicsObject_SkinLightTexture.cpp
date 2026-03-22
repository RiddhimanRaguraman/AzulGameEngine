//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_SkinLightTexture.h"
#include "CameraNodeMan.h"
#include "GameMan.h"
#include "TexNodeMan.h"

namespace Azul
{

	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_SkinLightTexture::GraphicsObject_SkinLightTexture(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		TextureObject::Name textName,
		ComputeBlend* _pBlend,
		Vec3& LightColor,
		Vec3& LightPos)
		: GraphicsObject(meshName, shaderName),
		pTex{ nullptr },
		poLightColor(nullptr),
		poLightPos(nullptr),
		poComputeBlend{ _pBlend }
	{
		this->pTex = TexNodeMan::Find(textName);
		assert(pTex);

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);

		assert(poLightColor);
		assert(poLightPos);
		assert(this->poComputeBlend);
	}

	GraphicsObject_SkinLightTexture::~GraphicsObject_SkinLightTexture()
	{
		delete poLightColor;
		delete poLightPos;
		delete this->poComputeBlend;
		this->poComputeBlend = nullptr;
	}

	void GraphicsObject_SkinLightTexture::SetState()
	{
		// Future - settings to directX
		// say make it wireframe or change culling mode
		this->pTex->ActivateTexture();
		GameMan::GetGame()->mStateRasterizerSolidCull.Activate();
	}

	void GraphicsObject_SkinLightTexture::SetDataGPU()
	{
		pMesh->ActivateMesh();
		pMesh->ActivateSRVBuffers();

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		Camera* pCam = CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		pShaderObj->TransferWorldViewProj(pCam, this->poWorld);
		pShaderObj->TransferPos(this->poLightPos);
		pShaderObj->TransferColor(this->poLightColor);
		// associate the world matrix
		this->poComputeBlend->BindWorldBoneArray();
	}

	void GraphicsObject_SkinLightTexture::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_SkinLightTexture::RestoreState()
	{
		GameMan::GetGame()->mBlendStateOff.Activate();
	}

}

// --- End of File ---
