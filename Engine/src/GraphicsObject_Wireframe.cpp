//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "Game.h"
#include "Engine.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_Wireframe.h"
#include "ShaderObject.h"
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
	GraphicsObject_Wireframe::GraphicsObject_Wireframe(Mesh::Name meshName,
		ShaderObject::Name shaderName,
		Vec3& rColor)
		: GraphicsObject(meshName, shaderName),
		poLightColor(nullptr)

	{
		assert(pMesh);
		assert(pShaderObj);
		assert(poWorld);

		this->poLightColor = new Vec3(rColor);
		assert(poLightColor);

	}

	GraphicsObject_Wireframe::~GraphicsObject_Wireframe()
	{
		delete this->poLightColor;
	}

	void GraphicsObject_Wireframe::SetState()
	{
		GameMan::GetGame()->mStateRasterizerWireNoCull.Activate();
	}

	void GraphicsObject_Wireframe::SetDataGPU()
	{
		pMesh->ActivateMesh();

		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();

		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), this->poWorld);
		pShaderObj->TransferColor(this->poLightColor);

		
	}

	void GraphicsObject_Wireframe::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_Wireframe::RestoreState()
	{
		GameMan::GetGame()->mStateRasterizerSolidCull.Activate();
	}

}

// ---  End of File ---
