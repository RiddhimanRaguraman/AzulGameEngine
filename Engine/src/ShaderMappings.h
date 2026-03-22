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

		xxxx,   //5  Skin
		xxx  //6  Skin
	};


	enum class ConstantPSBufferSlot : uint32_t
	{
		psSpriteUVMatrix,   // 0
		psSpriteColorScale, // 1

		psColor,            // 2 

	};

	// in shaders these are CBV slot "bx"
	enum class ConstantCSBufferSlot : uint32_t
	{
		xxxx,        // 0 
		csWorld,     // 1
		csMixerA,    // 2
		csMixerB,    // 3
		csMixerC     // 4
	};

	// in shaders these are SRV slot "tx"
	enum class ShaderResourceBufferSlot : uint32_t
	{
		Texture,         // 0
		KeyAa,           // 1  
		KeyAb,           // 2  
		HierarchyTable,  // 3
		BoneWorldIn,     // 4
		InvBind,         // 5
		KeyBa,           // 6 
		KeyBb            // 7  
	};

	// in shaders these are UAV slot "ux"
	enum class UnorderedAccessBufferSlot : uint32_t
	{
		xxxx,           // 0   
		BoneWorldOut,   // 1  
		MixerOutAx,     // 2 
		MixerOutBx,     // 3  
		MixerOutCx,     // 4  
	};

	// in shaders these are Sampler slot "sx"
	enum class ShaderSamplerSlot : uint32_t
	{
		SampleA   // slot 0
	};


}

#endif

// --- End of File ---
