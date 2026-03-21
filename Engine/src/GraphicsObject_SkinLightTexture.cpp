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
#include "StateDirectXMan.h"

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
		Mixer *_pMixer,
		WorldCompute *_pWorldCompute,
		Vec3& LightColor,
		Vec3& LightPos)
		: GraphicsObject(meshName, shaderName),
		pTex{ nullptr },
		poLightColor(nullptr),
		poLightPos(nullptr),
		pMixer{ _pMixer },
		pWorldCompute{ _pWorldCompute }
	{
		this->pTex = TexNodeMan::Find(textName);
		assert(pTex);

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);

		assert(poLightColor);
		assert(poLightPos);
		assert(pMixer);
		assert(pWorldCompute);
	}

	GraphicsObject_SkinLightTexture::~GraphicsObject_SkinLightTexture()
	{
		delete poLightColor;
		delete poLightPos;
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
		pMesh->ActivateConstantBuffers();
		//pMesh->Transfer_SkinBoneWorldBuffer(this->pBoneWorld);

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		Camera* pCam = CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		pShaderObj->TransferWorldViewProj(pCam, this->poWorld);
		pShaderObj->TransferPos(this->poLightPos);
		pShaderObj->TransferColor(this->poLightColor);
		StateDirectXMan::GetContext()->VSSetShaderResources(4, 1, &pWorldCompute->GetBoneWorld()->poShaderResourceView);

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
