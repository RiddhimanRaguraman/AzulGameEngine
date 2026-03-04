//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_ConstColor.h"
#include "Engine.h"
#include "StateDirectXMan.h"
#include "CameraNodeMan.h"
#include "GameMan.h"

namespace Azul
{
	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_ConstColor::GraphicsObject_ConstColor(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		Vec3& LightColor)
		: GraphicsObject(meshName, shaderName),
		poLightColor(nullptr)
	{
		poLightColor = new Vec3(LightColor);
		assert(poLightColor);
	}

	GraphicsObject_ConstColor::~GraphicsObject_ConstColor()
	{
		delete poLightColor;
	}

	void GraphicsObject_ConstColor::SetState()
	{
		GameMan::GetGame()->mStateRasterizerWireCull.Activate();
	}

	void GraphicsObject_ConstColor::SetDataGPU()
	{
		pMesh->ActivateMesh();
		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), this->poWorld);
		pShaderObj->TransferColor(this->poLightColor);

		
	}

	void GraphicsObject_ConstColor::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_ConstColor::RestoreState()
	{
		GameMan::GetGame()->mStateRasterizerSolidCull.Activate();
	}

}

// --- End of File ---
