//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_SkinLightTexture.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "SkinLightTexture.Px.h"
#include "SkinLightTexture.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_SkinLightTextureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t) VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT , (uint32_t)VertexSlot::Weights, offsetof(VertexWeights,Weight),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT  , (uint32_t)VertexSlot::Joints, offsetof(VertexJoints,Joint),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_SkinLightTexture::~ShaderObject_SkinLightTexture()
	{
	}

	ShaderObject_SkinLightTexture::ShaderObject_SkinLightTexture(ShaderObject_SkinLightTexture::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection{ sizeof(Mat4) },
		ConstantBuff_World{sizeof(Mat4)},
		ConstantBuff_View{ sizeof(Mat4) },
		ConstantBuff_LightColor{ sizeof(Vec3) },
		ConstantBuff_LightPos{ sizeof(Vec3) },

		VertexShader{ sizeof(g_SkinLightTexture_VxShader), (void*)g_SkinLightTexture_VxShader },
		PixelShader{ sizeof(g_SkinLightTexture_PxShader),(void*)g_SkinLightTexture_PxShader },
		InputLayout{ sizeof(g_SkinLightTexture_VxShader),(void*)g_SkinLightTexture_VxShader,ShaderObject_SkinLightTextureDesc,sizeof(ShaderObject_SkinLightTextureDesc) }
	{
	}

	void ShaderObject_SkinLightTexture::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_SkinLightTexture::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantVSBufferSlot::vsWorld);
		this->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		this->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);
		this->ConstantBuff_LightColor.SetActive(ConstantVSBufferSlot::vsLightColor);
		this->ConstantBuff_LightPos.SetActive(ConstantVSBufferSlot::vsLightPos);
	}

	void ShaderObject_SkinLightTexture::TransferWorldViewProj(Camera* pCam, Mat4* pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_SkinLightTexture::TransferPos(Vec3* pLightPos)
	{
		assert(pLightPos);

		this->ConstantBuff_LightPos.Transfer(pLightPos);
	}

	void ShaderObject_SkinLightTexture::TransferColor(Vec3* pColor)
	{
		assert(pColor);

		this->ConstantBuff_LightColor.Transfer(pColor);
	}

}

// --- End of File ---
