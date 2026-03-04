//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_ConstColorLight.h"
#include "Mesh.h"
#include "StateDirectXMan.h"

// User generated headers... 
#include "ConstColorLight.px.h"
#include "ConstColorLight.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_ConstantColorLightDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t) VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	ShaderObject_ConstColorLight::~ShaderObject_ConstColorLight()
	{

	}

	ShaderObject_ConstColorLight::ShaderObject_ConstColorLight(ShaderObject_ConstColorLight::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection { sizeof(Mat4) },
		ConstantBuff_World { sizeof(Mat4) },
		ConstantBuff_View { sizeof(Mat4) },

		ConstantBuff_Color { sizeof(Vec3) },
		ConstantBuff_LightPos { sizeof(Vec3) },
		ConstantBuff_BodyColor{ sizeof(Vec3) },
		VertexShader { sizeof(g_ConstColorLight_VxShader), (void *)g_ConstColorLight_VxShader },
		PixelShader { sizeof(g_ConstColorLight_PxShader),(void *)g_ConstColorLight_PxShader },
		InputLayout { sizeof(g_ConstColorLight_VxShader),(void *)g_ConstColorLight_VxShader, ShaderObject_ConstantColorLightDesc,sizeof(ShaderObject_ConstantColorLightDesc) }

	{

	}

	void ShaderObject_ConstColorLight::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_ConstColorLight::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantVSBufferSlot::vsWorld);
		this->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		this->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);

		this->ConstantBuff_Color.SetActive(ConstantVSBufferSlot::vsLightColor);
		this->ConstantBuff_LightPos.SetActive(ConstantVSBufferSlot::vsLightPos);

		this->ConstantBuff_BodyColor.SetActive(ConstantPSBufferSlot::psColor);
	}

	void ShaderObject_ConstColorLight::TransferWorldViewProj(Camera *pCam, Mat4 *pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_ConstColorLight::TransferPos(Vec3 *pLightPos)
	{
		assert(pLightPos);

		this->ConstantBuff_LightPos.Transfer(pLightPos);
	}

	void ShaderObject_ConstColorLight::TransferColor(Vec3 *pColor)
	{
		assert(pColor);

		this->ConstantBuff_Color.Transfer(pColor);
	}

	void ShaderObject_ConstColorLight::TransferBodyColor(Color* pColor)
	{
		assert(pColor);

		this->ConstantBuff_BodyColor.Transfer(pColor);
	}

}

// --- End of File ---
