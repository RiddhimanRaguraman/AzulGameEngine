//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix
//#define CBV_vsSkinInvBind

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_joint
#define VERTEX_weight

#define SRV_tSkinBoneWorld
#define SRV_tSkinInvBind

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

    row_major matrix SkinWorld = mul(mul(tSkinInvBind[inJoint.j.x].m, tSkinBoneWorld[inJoint.j.x].m), inWeight.w.x) +
	                    mul(mul(tSkinInvBind[inJoint.j.y].m, tSkinBoneWorld[inJoint.j.y].m), inWeight.w.y) +
                        mul(mul(tSkinInvBind[inJoint.j.z].m, tSkinBoneWorld[inJoint.j.z].m), inWeight.w.z) +
                        mul(mul(tSkinInvBind[inJoint.j.w].m, tSkinBoneWorld[inJoint.j.w].m), inWeight.w.w);
        
    // Mat = Skin * World * View * Proj
    row_major matrix Mat = mul(mul(mul(SkinWorld, vsWorldMatrix), vsViewMatrix), vsProjectionMatrix);
	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
    
    //  out = pos * Skin * View * Proj
    
    outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
    outValue.tex = inTex.tex;

    return outValue;
}

// --- End of File ---
