#define SAMPLER_sA
#define SRV_tMainTexture

#include "ShaderMappings.hlsli"

// -------------------------------------
//  Pixel Shader Input Sematics
//        COLOR - Input to the Pixel Shader
//        TEXCOORD - uv tex coord
// -------------------------------------
struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 main(VertexShaderOutput inData) : SV_TARGET
{
    return inData.color * tMainTexture.Sample(sSamplerA, inData.tex);
}

