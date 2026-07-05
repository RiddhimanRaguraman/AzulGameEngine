//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_Sprite.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "Sprite.Px.h"
#include "Sprite.Vx.h"

namespace Azul
{
	D3D11_INPUT_ELEMENT_DESC ShaderObject_SpriteDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderObject_Sprite::~ShaderObject_Sprite()
	{
	}

	ShaderObject_Sprite::ShaderObject_Sprite(ShaderObject_Sprite::Name _name)
		: ShaderObject(_name),
		ConstantBuff_Projection{ sizeof(Mat4) },
		ConstantBuff_World{ sizeof(Mat4) },
		ConstantBuff_View{ sizeof(Mat4) },
		ConstantBuff_UVMatrix { sizeof(Mat4) },
		ConstantBuff_ColorScale{ sizeof(Vec4) },
		VertexShader{ sizeof(g_Sprite_VxShader), (void*)g_Sprite_VxShader },
		PixelShader{ sizeof(g_Sprite_PxShader),(void*)g_Sprite_PxShader },
		InputLayout{ sizeof(g_Sprite_VxShader),(void*)g_Sprite_VxShader,ShaderObject_SpriteDesc,sizeof(ShaderObject_SpriteDesc) }
	{
	}

	void ShaderObject_Sprite::ActivateShader()
	{
		this->VertexShader.SetActive();
		this->InputLayout.SetActive();
		this->PixelShader.SetActive();
	}

	void ShaderObject_Sprite::ActivateCBV()
	{
		this->ConstantBuff_World.SetActive(ConstantVSBufferSlot::vsWorld);
		this->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		this->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);
		this->ConstantBuff_UVMatrix.SetActive(ConstantPSBufferSlot::psSpriteUVMatrix);
		this->ConstantBuff_ColorScale.SetActive(ConstantPSBufferSlot::psSpriteColorScale);
	}

	void ShaderObject_Sprite::TransferWorldViewProj(Camera* pCam, Mat4* pWorld)
	{
		assert(pCam);
		assert(pWorld);

		this->ConstantBuff_World.Transfer(pWorld);
		this->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		this->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
	}

	void ShaderObject_Sprite::TransferUVMatrix(Mat4* pUVMatrix)
	{
		assert(pUVMatrix);
		this->ConstantBuff_UVMatrix.Transfer(pUVMatrix);
	}

	void ShaderObject_Sprite::TransferColorScale(Color* pScaleColor)
	{
		assert(pScaleColor);

		this->ConstantBuff_ColorScale.Transfer((Vec4*)pScaleColor);
	}
}

// --- End of File ---
