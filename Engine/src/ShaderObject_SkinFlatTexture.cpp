//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_SkinFlatTexture.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "SkinFlatTexture.Px.h"
#include "SkinFlatTexture.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_SkinFlatTextureDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT , (uint32_t)VertexSlot::Weights, offsetof(VertexWeights,Weight),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT  , (uint32_t)VertexSlot::Joints, offsetof(VertexJoints,Joint),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_SkinFlatTexture::~ShaderObject_SkinFlatTexture()
	{
	}

	ShaderObject_SkinFlatTexture::ShaderObject_SkinFlatTexture(ShaderObject_SkinFlatTexture::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection{ sizeof(Mat4) },
		//ConstantBuff_World{sizeof(Mat4)},
		ConstantBuff_View{ sizeof(Mat4) },

		VertexShader{ sizeof(g_SkinFlatTexture_VxShader), (void*)g_SkinFlatTexture_VxShader },
		PixelShader{ sizeof(g_SkinFlatTexture_PxShader),(void*)g_SkinFlatTexture_PxShader },
		InputLayout{ sizeof(g_SkinFlatTexture_VxShader),(void*)g_SkinFlatTexture_VxShader,ShaderObject_SkinFlatTextureDesc,sizeof(ShaderObject_SkinFlatTextureDesc) }
	{
	}

	void ShaderObject_SkinFlatTexture::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_SkinFlatTexture::ActivateCBV()
	{
		//	this->ConstantBuff_World.SetActive(ConstantBufferSlot::vsWorld);
		this->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		this->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);
	}

	void ShaderObject_SkinFlatTexture::TransferWorldViewProj(Camera* pCam, Mat4* pWorld)
	{
		assert(pCam);
		assert(pWorld);

		//	this->ConstantBuff_World.Transfer(pWorld);
		this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

}

// --- End of File ---