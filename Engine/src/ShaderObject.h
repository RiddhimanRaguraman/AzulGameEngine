//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <d3d11.h>
#include "MathEngine.h"
#include "Camera.h"
#include "Color.h"

namespace Azul
{
	class ShaderObject
	{
	public:
		enum class Name
		{
			NullShader,
			ColorByVertex,
			FlatTexture,
			LightTexture,
			BasicCompute,
			SkinFlatTexture,
			SkinLightTexture,
			WorldCompute,
			MixerCompute,
			ConstColor,
			Sprite,
			Uninitialized
		};


	public:
		ShaderObject() = delete;
		ShaderObject(const ShaderObject &) = delete;
		ShaderObject &operator = (const ShaderObject &) = delete;
		virtual ~ShaderObject();

		ShaderObject(ShaderObject::Name _name);

		virtual void ActivateShader() = 0;
		virtual void ActivateCBV();
		virtual void TransferWorldViewProj(Camera *pCam, Mat4 *pWorld);
		virtual void TransferPos(Vec3 *pLightPos);
		virtual void TransferColor(Vec3 *pColor);
		virtual void TransferBodyColor(Color* pColor);
		virtual void TransferUVMatrix(Mat4* pMat);
		virtual void TransferColorScale(Color* pScaleColor);

		static const char *GetLatestProfile_PixelShader();
		static const char *GetLatestProfile_VertexShader();
		static const char* GetLatestProfile_ComputeShader();

		char *GetName();
		void SetName(ShaderObject::Name _name);

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		ShaderObject::Name name;

	};
}

#endif

// --- End of File ---
