//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "Color.h"
#include "GraphicsObject_ConstColorLight.h"
#include "CameraNodeMan.h"

namespace Azul
{

	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_ConstColorLight::GraphicsObject_ConstColorLight(Mesh::Name meshName,
															 ShaderObject::Name shaderName,
															 Vec3 &LightColor,
															 Vec3 &LightPos,
															 Vec3 &BodyColor)
		: GraphicsObject(meshName, shaderName),
		poLightColor(nullptr),
		poLightPos(nullptr)
	{

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);
		poBodyColor = new Vec3(BodyColor);

		assert(poLightColor);
		assert(poLightPos);
	}

	GraphicsObject_ConstColorLight::~GraphicsObject_ConstColorLight()
	{
		delete poLightColor;
		delete poLightPos;
		delete poBodyColor;
	}

	void GraphicsObject_ConstColorLight::SetState()
	{

	}

	void GraphicsObject_ConstColorLight::SetDataGPU()
	{
		pMesh->ActivateMesh();

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();
		

		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D) , this->poWorld);
		pShaderObj->TransferPos(this->poLightPos);
		pShaderObj->TransferColor(this->poLightColor);

		Color col(*this->poBodyColor);
		pShaderObj->TransferBodyColor(&col);

		

	}

	void GraphicsObject_ConstColorLight::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_ConstColorLight::RestoreState()
	{
		// Future - Undo settings to directX
	}

}

// --- End of File ---
