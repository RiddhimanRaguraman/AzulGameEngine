//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_CONST_COLOR_LIGHT_H
#define SHADER_OBJECT_CONST_COLOR_LIGHT_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferCBV_ps.h"
#include "BufferVertexShader_vs.h"
#include "BufferPixelShader_ps.h"
#include "BufferInputLayout_ia.h"

namespace Azul
{
	class ShaderObject_ConstColorLight : public ShaderObject
	{
	public:
		ShaderObject_ConstColorLight() = delete;
		ShaderObject_ConstColorLight(const ShaderObject_ConstColorLight &) = delete;
		ShaderObject_ConstColorLight &operator = (const ShaderObject_ConstColorLight &) = delete;
		~ShaderObject_ConstColorLight();

		ShaderObject_ConstColorLight(ShaderObject_ConstColorLight::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera *pCam, Mat4 *pWorld) override;
		virtual void TransferPos(Vec3 *pLightPos) override;
		virtual void TransferColor(Vec3 *pColor) override;

		virtual  void TransferBodyColor(Color* pColor) override;


		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferCBV_vs ConstantBuff_Projection;
		BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;

		BufferCBV_vs ConstantBuff_Color;
		BufferCBV_vs ConstantBuff_LightPos;

		BufferCBV_ps ConstantBuff_BodyColor;

		BufferVertexShader_vs VertexShader;
		BufferPixelShader_ps  PixelShader;
		BufferInputLayout_ia  InputLayout;
	};

}
#endif

// --- End of File ---
