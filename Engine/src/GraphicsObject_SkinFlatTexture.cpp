//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_SkinFlatTexture.h"
#include "CameraNodeMan.h"
#include "GameMan.h"
#include "TexNodeMan.h"
//#include "BufferSRV_cs.h"

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
		ComputeBlend* _pBlend)
		: GraphicsObject(meshName, shaderName),
		pTex{ nullptr }, 
		poComputeBlend{ _pBlend }
	{
		this->pTex = TexNodeMan::Find(textName);
		assert(this->pTex);
		assert(this->poComputeBlend);
	}

	GraphicsObject_SkinFlatTexture::~GraphicsObject_SkinFlatTexture()
	{
		delete this->poComputeBlend;
		this->poComputeBlend = nullptr;
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
		pMesh->ActivateSRVBuffers();

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		Camera *pCam = CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		assert(pCam);

		pShaderObj->TransferWorldViewProj(pCam, this->poWorld);

		// associate the world matrix
		this->poComputeBlend->BindWorldBoneArray();


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
