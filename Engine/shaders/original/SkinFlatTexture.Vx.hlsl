//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsSkinInvBind
#define CBV_vsSkinBoneWorld

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_joint
#define VERTEX_weight

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

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput main(VertData_pos inPos,
                        VertData_tex inTex,
                        VertData_weight inWeight,
                        VertData_joint inJoint)
{
    VertexShaderOutput outValue;

    matrix SkinWorld = mul(mul(vsSkinInvBind[inJoint.j.x], vsSkinBoneWorld[inJoint.j.x]), inWeight.w.x) +
	                    mul(mul(vsSkinInvBind[inJoint.j.y], vsSkinBoneWorld[inJoint.j.y]), inWeight.w.y) +
                        mul(mul(vsSkinInvBind[inJoint.j.z], vsSkinBoneWorld[inJoint.j.z]), inWeight.w.z) +
                        mul(mul(vsSkinInvBind[inJoint.j.w], vsSkinBoneWorld[inJoint.j.w]), inWeight.w.w);
    
    matrix Mat = mul(mul(SkinWorld, vsViewMatrix), vsProjectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
    
    //  out = pos * Skin * View * Proj
    
    outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
    outValue.tex = inTex.tex;

    return outValue;
}

// --- End of File ---
