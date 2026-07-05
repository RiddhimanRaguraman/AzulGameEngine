//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_LightTexture.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"

namespace Azul
{
	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_LightTexture::GraphicsObject_LightTexture(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		TextureObject::Name texName,
		Vec3& LightColor,
		Vec3& LightPos)
		: GraphicsObject(meshName, shaderName),
		pTex(TexNodeMan::Find(texName)),
		poLightColor(nullptr),
		poLightPos(nullptr)
	{
		assert(pTex);

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);

		assert(poLightColor);
		assert(poLightPos);
	}

	GraphicsObject_LightTexture::GraphicsObject_LightTexture(Mesh* pMesh,
		ShaderObject::Name shaderName,
		TextureObject::Name texName,
		Vec3& LightColor,
		Vec3& LightPos)
		: GraphicsObject(pMesh->name, shaderName),
		pTex(TexNodeMan::Find(texName)),
		poLightColor(nullptr),
		poLightPos(nullptr)
	{
		assert(pTex);
		assert(pMesh);

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);

		assert(poLightColor);
		assert(poLightPos);
	}

	GraphicsObject_LightTexture::~GraphicsObject_LightTexture()
	{
		delete poLightColor;
		delete poLightPos;
	}

	void GraphicsObject_LightTexture::SetState()
	{
		// Future - settings to directX
		// say make it wireframe or change culling mode
		this->pTex->ActivateTexture();
	}

	void GraphicsObject_LightTexture::SetDataGPU()
	{
		pMesh->ActivateMesh();

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), this->poWorld);
		pShaderObj->TransferPos(this->poLightPos);
		pShaderObj->TransferColor(this->poLightColor);

		

	}

	void GraphicsObject_LightTexture::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_LightTexture::RestoreState()
	{
		// Future - Undo settings to directX
	}

}

// --- End of File ---
