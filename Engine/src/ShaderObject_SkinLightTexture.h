//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_SKIN_LIGHT_TEXTURE_H
#define SHADER_OBJECT_SKIN_LIGHT_TEXTURE_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferVertexShader_vs.h"
#include "BufferPixelShader_ps.h"
#include "BufferInputLayout_ia.h"

namespace Azul
{
	class ShaderObject_SkinLightTexture : public ShaderObject
	{
	public:
		ShaderObject_SkinLightTexture() = delete;
		ShaderObject_SkinLightTexture(const ShaderObject_SkinLightTexture&) = delete;
		ShaderObject_SkinLightTexture& operator = (const ShaderObject_SkinLightTexture&) = delete;
		virtual ~ShaderObject_SkinLightTexture();

		ShaderObject_SkinLightTexture(ShaderObject_SkinLightTexture::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera* pCam, Mat4* pWorld) override;
		virtual void TransferPos(Vec3* pLightPos) override;
		virtual void TransferColor(Vec3* pColor) override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferCBV_vs ConstantBuff_Projection;
		//	BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;
		
		BufferCBV_vs ConstantBuff_LightColor;
		BufferCBV_vs ConstantBuff_LightPos;

		BufferVertexShader_vs VertexShader;
		BufferPixelShader_ps  PixelShader;
		BufferInputLayout_ia  InputLayout;

	};
}

#endif

// --- End of File ---
