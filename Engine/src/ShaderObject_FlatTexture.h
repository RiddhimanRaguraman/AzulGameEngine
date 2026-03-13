//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_FLAT_TEXTURE_H
#define SHADER_OBJECT_FLAT_TEXTURE_H

#include "ShaderObject.h"
#include "BufferCBV_vs.h"
#include "BufferVertexShader_vs.h"
#include "BufferPixelShader_ps.h"
#include "BufferInputLayout_ia.h"

namespace Azul
{
	class ShaderObject_FlatTexture : public ShaderObject
	{
	public:
		ShaderObject_FlatTexture() = delete;
		ShaderObject_FlatTexture(const ShaderObject_FlatTexture &) = delete;
		ShaderObject_FlatTexture &operator = (const ShaderObject_FlatTexture &) = delete;
		virtual ~ShaderObject_FlatTexture();

		ShaderObject_FlatTexture(ShaderObject_FlatTexture::Name _name);

		virtual void ActivateShader() override;
		virtual void ActivateCBV() override;
		virtual void TransferWorldViewProj(Camera *pCam, Mat4 *pWorld) override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferCBV_vs ConstantBuff_Projection;
		BufferCBV_vs ConstantBuff_World;
		BufferCBV_vs ConstantBuff_View;

		BufferVertexShader_vs VertexShader;
		BufferPixelShader_ps  PixelShader;
		BufferInputLayout_ia  InputLayout;

	};
}

#endif

// --- End of File ---
