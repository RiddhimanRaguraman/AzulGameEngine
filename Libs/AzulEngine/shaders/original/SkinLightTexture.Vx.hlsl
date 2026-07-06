// Enable data
#define CBV_vsProjectionMatrix
#define CBV_vsViewMatrix
#define CBV_vsWorldMatrix
#define CBV_vsLightColor
#define CBV_vsLightPos
//#define CBV_vsSkinInvBind
//#define CBV_vsSkinBoneWorld

#define VERTEX_pos
#define VERTEX_tex
#define VERTEX_norm
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

    row_major matrix SkinWorld = mul(mul(tSkinInvBind[inJoint.j.x].m, tSkinBoneWorld[inJoint.j.x].m), inWeight.w.x) +
	                    mul(mul(tSkinInvBind[inJoint.j.y].m, tSkinBoneWorld[inJoint.j.y].m), inWeight.w.y) +
                        mul(mul(tSkinInvBind[inJoint.j.z].m, tSkinBoneWorld[inJoint.j.z].m), inWeight.w.z) +
                        mul(mul(tSkinInvBind[inJoint.j.w].m, tSkinBoneWorld[inJoint.j.w].m), inWeight.w.w);
    
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
    const float ambient = 0.35f;
    outValue.color.xyz = vsLightColor.xyz * (ambient + (1.0f - ambient) * fDot);
    outValue.color.w = 1.0f;
    
    return outValue;
}

// --- End of File ---
