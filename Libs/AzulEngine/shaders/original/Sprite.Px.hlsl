//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define SAMPLER_sA
#define SRV_tMainTexture
#define CBV_psUVMatrix
#define CBV_psColorScale

#include "ShaderMappings.hlsli"

// -------------------------------------
//  Pixel Shader Input Sematics
//        TEXCOORD - uv tex coord
//      Note: output layout needs to be the same for input input pixel
// -------------------------------------
struct PixelShaderInput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 main(PixelShaderInput inData ) : SV_TARGET
{
	// correct the UV coordinates, originally its (0,0 to 1,1)
	// scale and translate to the correct sub image on the texture

    float4 uv_vec;
    uv_vec.xy = inData.tex.xy;
    uv_vec.z = 0.0f;
    uv_vec.w = 1.0f;

    float4 uv_corrected = mul(uv_vec, psUVMatrix);

    return psColorScale * tMainTexture.Sample(sSamplerA, uv_corrected.xy);
}


// --- End of File ---
