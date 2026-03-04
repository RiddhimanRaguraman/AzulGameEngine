//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix
#define CBV_vsLightColor
#define CBV_vsLightPos

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_norm

#include "ShaderMappings.hlsli"

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
// ------------------------------------------------------------
struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput main(VertData_pos inPos, VertData_tex inTex, VertData_norm inNorm)
{
    VertexShaderOutput outValue;

	// Mat = World * View * Proj
    matrix Mat = mul(mul(vsWorldMatrix, vsViewMatrix), vsProjectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
    outValue.position = mul(float4(inPos.pos, 1.0f), Mat);

	// Texture
    outValue.tex = inTex.tex;

	// color calculation
    matrix mvMatrix = mul(vsWorldMatrix, vsViewMatrix);
    float3x3 mNormalMatrix;

    mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
    mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
    mNormalMatrix[2] = normalize(mvMatrix[2].xyz);
    float3 vA = mul(inNorm.norm.xyz, mNormalMatrix);
    float3 vNorm = normalize(vA);

	// Get vLightDir
    float4 ecPosition;
    float3 ecPosition3;
    ecPosition = mul(float4(inPos.pos, 1.0f), mvMatrix);
    ecPosition3 = ecPosition.xyz / ecPosition.w;
    float3 vLightDir = normalize(vsLightPos.xyz - ecPosition3);

	// Get vFragColor
    float fDot = max(0.0, dot(vNorm, vLightDir));
    outValue.color.xyz = vsLightColor.xyz * fDot;
    outValue.color.w = 1.0f;

    return outValue;
}


// --- End of File ---

