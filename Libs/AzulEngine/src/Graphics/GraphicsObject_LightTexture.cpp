//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_LightTexture.h"
#include "CameraNodeMan.h"
#include "TexNodeMan.h"
#include "WorldMan.h"
#include "World.h"
#include "LightComponent.h"

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
		pTex(TexNodeMan::Find(texName))
	{
		assert(pTex);
		this->privInitLight(LightColor, LightPos);
	}

	GraphicsObject_LightTexture::GraphicsObject_LightTexture(Mesh* pMesh,
		ShaderObject::Name shaderName,
		TextureObject::Name texName,
		Vec3& LightColor,
		Vec3& LightPos)
		: GraphicsObject(pMesh->name, shaderName),
		pTex(TexNodeMan::Find(texName))
	{
		assert(pTex);
		assert(pMesh);
		this->privInitLight(LightColor, LightPos);
	}

	void GraphicsObject_LightTexture::privInitLight(Vec3& color, Vec3& pos)
	{
		this->mLightEntity = WorldMan::GetWorld().Create();
		LightComponent& light = WorldMan::GetWorld().Add<LightComponent>(this->mLightEntity);
		light.color = color;
		light.pos = pos;
	}

	GraphicsObject_LightTexture::~GraphicsObject_LightTexture()
	{
		WorldMan::GetWorld().Destroy(this->mLightEntity);
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

		LightComponent *pLight = WorldMan::GetWorld().TryGet<LightComponent>(this->mLightEntity);
		assert(pLight);
		pShaderObj->TransferPos(&pLight->pos);
		pShaderObj->TransferColor(&pLight->color);

		

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
