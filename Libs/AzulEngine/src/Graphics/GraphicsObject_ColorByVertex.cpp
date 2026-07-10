//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_ColorByVertex.h"
#include "CameraNodeMan.h"

namespace Azul
{
	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GraphicsObject_ColorByVertex::GraphicsObject_ColorByVertex(Mesh::Name meshName,
		ShaderObject::Name shaderName)
		: GraphicsObject(meshName, shaderName)
	{

	}

	void GraphicsObject_ColorByVertex::SetState()
	{
		// Future - settings to directX
		// say make it wireframe or change culling mode
	}

	void GraphicsObject_ColorByVertex::SetDataGPU()
	{
		pMesh->ActivateMesh();
		pShaderObj->ActivateShader();
		pShaderObj->ActivateCBV();
		pShaderObj->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), this->poWorld);
	}

	void GraphicsObject_ColorByVertex::Draw()
	{
		pMesh->RenderIndexBuffer();
	}

	void GraphicsObject_ColorByVertex::RestoreState()
	{
		// Future - Undo settings to directX
	}

}

// --- End of File ---
