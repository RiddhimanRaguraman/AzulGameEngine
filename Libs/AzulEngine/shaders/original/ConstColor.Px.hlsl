//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_psColor

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

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 main(VertexShaderOutput inData ) : SV_TARGET
{
	 // make alpha=1
    return float4(psColor.xyz, 1.0f);
}

// --- End of File ---
