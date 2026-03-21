// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix
#define CBV_vsLightColor
#define CBV_vsLightPos
#define CBV_vsSkinInvBind
//#define CBV_vsSkinBoneWorld

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_norm
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
    float4 color : COLOR;
    float4 position : SV_POSITION;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput main(VertData_pos inPos,
                        VertData_tex inTex, VertData_norm inNorm,
                        VertData_weight inWeight,
                        VertData_joint inJoint)
{
    VertexShaderOutput outValue;

    row_major matrix SkinWorld = mul(mul(vsSkinInvBind[inJoint.j.x], SkinBoneWorld[inJoint.j.x].m), inWeight.w.x) +
	                            mul(mul(vsSkinInvBind[inJoint.j.y], SkinBoneWorld[inJoint.j.y].m), inWeight.w.y) +
                                mul(mul(vsSkinInvBind[inJoint.j.z], SkinBoneWorld[inJoint.j.z].m), inWeight.w.z) +
                                mul(mul(vsSkinInvBind[inJoint.j.w], SkinBoneWorld[inJoint.j.w].m), inWeight.w.w);
    
    // Skin * World * View * Proj
    matrix Mat = mul(mul(mul(SkinWorld, vsWorldMatrix), vsViewMatrix), vsProjectionMatrix);
	
	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
    
    //  out = pos * Skin * View * Proj
    
    outValue.position = mul(float4(inPos.pos, 1.0f), Mat);
    outValue.tex = inTex.tex;

    // color calculation
    matrix mvMatrix = mul(SkinWorld, vsViewMatrix);
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
