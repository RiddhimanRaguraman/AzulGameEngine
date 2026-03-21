//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix
#define CBV_vsSkinInvBind
//#define CBV_vsSkinBoneWorld

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_joint
#define VERTEX_weight

#include "ShaderMappings.hlsli"

StructuredBuffer<rowMatrix> SkinBoneWorld : register(t4); // slot 4  

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

    row_major matrix SkinWorld = mul(mul(vsSkinInvBind[inJoint.j.x], SkinBoneWorld[inJoint.j.x].m), inWeight.w.x) +
	                    mul(mul(vsSkinInvBind[inJoint.j.y], SkinBoneWorld[inJoint.j.y].m), inWeight.w.y) +
                        mul(mul(vsSkinInvBind[inJoint.j.z], SkinBoneWorld[inJoint.j.z].m), inWeight.w.z) +
                        mul(mul(vsSkinInvBind[inJoint.j.w], SkinBoneWorld[inJoint.j.w].m), inWeight.w.w);
        
    // Skin * World * View * Proj
    row_major matrix Mat = mul(mul(mul(SkinWorld, vsWorldMatrix), vsViewMatrix), vsProjectionMatrix);
	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
    
    //  out = pos * Skin * View * Proj
    
    outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
    outValue.tex = inTex.tex;

    return outValue;
}

// --- End of File ---
