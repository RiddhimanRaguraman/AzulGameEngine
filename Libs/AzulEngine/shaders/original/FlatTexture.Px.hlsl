//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define SAMPLER_sA
#define SRV_tMainTexture
#define CBV_psUVMatrix

#include "ShaderMappings.hlsli"

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
//      Note: output layout needs to be the same for input input pixel
// ------------------------------------------------------------
struct VertexShaderOutput
{
	float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 main(VertexShaderOutput inData) : SV_TARGET
{
    float4 uv_vec;
    uv_vec.xy = inData.tex.xy;
    uv_vec.z = 0.0f;
    uv_vec.w = 1.0f;

    float4 uv_corrected = mul(uv_vec, psUVMatrix);

    return tMainTexture.Sample(sSamplerA, uv_corrected.xy);
}


// --- End of File ---
