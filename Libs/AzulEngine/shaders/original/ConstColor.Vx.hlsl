//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix

#define VERTEX_pos

#include "ShaderMappings.hlsli"

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
// ------------------------------------------------------------
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput main(VertData_pos inPos)
{
	VertexShaderOutput outValue;

	// Mat = World * View * Proj
    matrix Mat = mul(mul(vsWorldMatrix, vsViewMatrix), vsProjectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
	return outValue;
}

// --- End of File ---
