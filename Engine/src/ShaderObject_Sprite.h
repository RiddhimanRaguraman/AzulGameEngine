//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_SPRITE_H
#define SHADER_OBJECT_SPRITE_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferCBV_ps.h"
#include "BufferVertexShader_vs.h"
#include "BufferPixelShader_ps.h"
#include "BufferInputLayout_ia.h"

namespace Azul
{
	class ShaderObject_Sprite : public ShaderObject
	{
	public:

	public:
		ShaderObject_Sprite() = delete;
		ShaderObject_Sprite(const ShaderObject_Sprite&) = delete;
		ShaderObject_Sprite& operator = (const ShaderObject_Sprite&) = delete;
		virtual ~ShaderObject_Sprite();

		ShaderObject_Sprite(ShaderObject_Sprite::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera* pCam, Mat4* pWorld) override;
		virtual void TransferUVMatrix(Mat4* pUVMatrix) override;
		virtual void TransferColorScale(Color* pScaleColor) override;
		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferCBV_vs ConstantBuff_Projection;
		BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;

		BufferCBV_ps ConstantBuff_UVMatrix;
		BufferCBV_ps ConstantBuff_ColorScale;

		BufferVertexShader_vs VertexShader;
		BufferPixelShader_ps  PixelShader;
		BufferInputLayout_ia  InputLayout;

	};
}

#endif

// --- End of File ---
