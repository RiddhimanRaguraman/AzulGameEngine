//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_MAPPINGS_H
#define SHADER_MAPPINGS_H

namespace Azul
{

	enum class VertexSlot : uint32_t
	{
		Position,   // 0
		Color,      // 1
		Norm,       // 2
		TexCoord,   // 3
		Weights,    // 4  Skin
		Joints      // 5  Skin
	};

	static const unsigned int BONE_COUNT_MAX = 46;

	// in shaders these are CBV slot "bx"
	enum class ConstantVSBufferSlot : uint32_t
	{
		vsProjection,   // 0
		vsView,         // 1
		vsWorld,        // 2

		vsLightColor,   // 3
		vsLightPos,     // 4

		vsSkinInvBind,   //5  Skin
		vsSkinBoneWorld  //6  Skin
	};


	enum class ConstantPSBufferSlot : uint32_t
	{
		psSpriteUVMatrix,   // 0
		psSpriteColorScale, // 1

		psColor,            // 2 

	};

	//enum class ConstantBufferSlot : uint32_t
	//{
	//	vsProjection,   // 0
	//	vsView,         // 1
	//	vsWorld,        // 2
	//	vsLightColor,   // 3
	//	vsLightPos,     // 4
	//	psSpriteUVMatrix,   // 5
	//	psSpriteColorScale, // 6
	//	psColor,         // 7
	//	vsSkinInvBind,       //8  Skin
	//	vsSkinBoneWorld      //9  Skin
	//};

	// in shaders these are RSV slot "tx"
	enum class ShaderResourceBufferSlot : uint32_t
	{
		TexA      // slot 0  
	};

	// in shaders these are Sampler slot "sx"
	enum class ShaderSamplerSlot : uint32_t
	{
		SampleA   // slot 0
	};


}

#endif

// --- End of File ---
