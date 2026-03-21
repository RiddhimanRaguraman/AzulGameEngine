//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_SkinFlatTexture.h"
#include "CameraNodeMan.h"
#include "GameMan.h"
#include "TexNodeMan.h"
#include "StateDirectXMan.h"
#include "BufferSRV_cs.h"
namespace Azul
{

	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_SkinFlatTexture::GraphicsObject_SkinFlatTexture(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		TextureObject::Name textName,
		Mixer *_pMixer,
		WorldCompute *_pWorldCompute)
		: GraphicsObject(meshName, shaderName),
		pTex{ nullptr }, pMixer{ _pMixer },
		pWorldCompute{ _pWorldCompute }
	{
		this->pTex = TexNodeMan::Find(textName);
		assert(pTex);
		assert(pMixer);
		assert(pWorldCompute);
	}


	void GraphicsObject_SkinFlatTexture::SetState()
	{
		// Future - settings to directX
		// say make it wireframe or change culling mode
		this->pTex->ActivateTexture();
		GameMan::GetGame()->mStateRasterizerSolidCull.Activate();
	}

	void GraphicsObject_SkinFlatTexture::SetDataGPU()
	{
		pMesh->ActivateMesh();
		pMesh->ActivateConstantBuffers();
		//pMesh->Transfer_SkinBoneWorldBuffer(this->pBoneWorld);

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		Camera *pCam = CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		pShaderObj->TransferWorldViewProj(pCam, this->poWorld);

		StateDirectXMan::GetContext()->VSSetShaderResources(4, 1, &pWorldCompute->GetBoneWorld()->poShaderResourceView);


	}

	void GraphicsObject_SkinFlatTexture::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_SkinFlatTexture::RestoreState()
	{
		GameMan::GetGame()->mBlendStateOff.Activate();
	}

}

// --- End of File ---