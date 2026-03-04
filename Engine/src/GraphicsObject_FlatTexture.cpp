//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_FlatTexture.h"
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
	GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		TextureObject::Name textName)
		: GraphicsObject(meshName, shaderName),
		pTex(nullptr)
	{
		this->pTex = TexNodeMan::Find(textName);
		assert(pTex);
	}


	void GraphicsObject_FlatTexture::SetState()
	{
		
		this->pTex->ActivateTexture();

		if (this->pTex->HasAlpha())
		{ //Alpha is printed only if the Texture has alpha
			GameMan::GetGame()->mBlendStateAlpha.Activate();
		}
	}

	void GraphicsObject_FlatTexture::SetDataGPU()
	{
		pMesh->ActivateMesh();
		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();
		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), this->poWorld);
		
	}

	void GraphicsObject_FlatTexture::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_FlatTexture::RestoreState()
	{
		GameMan::GetGame()->mBlendStateOff.Activate();
	}

}

// --- End of File ---
