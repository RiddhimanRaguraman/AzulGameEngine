//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_ColorByVertex.h"
#include "Mesh.h"
#include "ShaderMappings.h"
#include "BufferCBV_vs.h"
#include "StateDirectXMan.h"

// User generated headers... 
#include "ColorByVertex.Px.h"
#include "ColorByVertex.Vx.h"

namespace Azul
{

	D3D11_INPUT_ELEMENT_DESC ShaderObject_ColorByVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Color, offsetof(VertexColor,Color),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_ColorByVertex::~ShaderObject_ColorByVertex()
	{
	}

	ShaderObject_ColorByVertex::ShaderObject_ColorByVertex(ShaderObject_ColorByVertex::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection { sizeof(Mat4) },
		ConstantBuff_World { sizeof(Mat4) },
		ConstantBuff_View { sizeof(Mat4) },
		VertexShader { sizeof(g_ColorByVertex_VxShader), (void *) g_ColorByVertex_VxShader },
		PixelShader { sizeof(g_ColorByVertex_PxShader),(void *) g_ColorByVertex_PxShader },
		InputLayout { sizeof(g_ColorByVertex_VxShader),(void *) g_ColorByVertex_VxShader,ShaderObject_ColorByVertexDesc,sizeof(ShaderObject_ColorByVertexDesc) }
	{
	}

	void ShaderObject_ColorByVertex::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_ColorByVertex::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantVSBufferSlot::vsWorld);
		this->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		this->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);
	}

	void ShaderObject_ColorByVertex::TransferWorldViewProj(Camera *pCam, Mat4 *pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

}
// --- End of File ---
