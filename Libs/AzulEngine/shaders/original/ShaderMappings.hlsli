//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
//
//  https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/d3d11-graphics-reference-sm5-system-values
//  https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/registers---vs-5-0
//  https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/registers---cs-5-0
//  https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/registers---ps-5-0
// -------------------------------------------------------------

#pragma pack_matrix( row_major )

// ---------------------------------------------
// Structures
// ---------------------------------------------

struct rowMatrix
{
    row_major matrix m;
};

struct BoneType
{
    float4 t;
    float4 q;
    float4 s;
};

struct MixerData
{
    float ts;
    uint numNodes;
};

struct WorldData
{
    uint hierarchyDepth;
    uint numBones;
};

//#define BONE_COUNT_MAX 60


// ------------------------------------------
// Vertex data s is sampler, x - slot
// ------------------------------------------

#ifdef VERTEX_pos
    struct VertData_pos  //  vertex constant buffer, slot 0 (VertexSlot::Position)
    {
        float3 pos : POSITION;
    };
#endif

#ifdef VERTEX_color
    struct VertData_color  // vertex constant buffer, slot 1 (VertexSlot::Color)
    {
        float3 color : COLOR;
    };
#endif

#ifdef VERTEX_norm
    struct VertData_norm  // vertex constant buffer, slot 2 (VertexSlot::Norm)
    {
        float3 norm : NORMAL; // norm location
    };
#endif

#ifdef VERTEX_tex
    struct VertData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
    {
        float2 tex : TEXCOORD; // uv location
    };
#endif

#ifdef VERTEX_weight
    struct VertData_weight  // vertex constant buffer, slot 4 (VertexSlot::Weights)
    {
        float4 w : BLENDWEIGHT; // weight location
    };
#endif

#ifdef VERTEX_joint
    struct VertData_joint  // vertex constant buffer, slot 5 (VertexSlot::Joints)
    {
        uint4 j : BLENDINDICES; // joints location
    };
#endif

// ------------------------------------------
// sx - s is sampler, x - slot
//      max count == 16
// ------------------------------------------

#ifdef SAMPLER_sA
    SamplerState sSamplerA : register(s0);
#endif

// --------------------------------------------------
// tx - t is input shader resource views (SRV), x - slot
//      max count == 128
// --------------------------------------------------

#ifdef SRV_tMainTexture
    Texture2D tMainTexture : register(t0);
#endif

#ifdef SRV_tKeyAa
    StructuredBuffer<BoneType> tKeyAa : register(t1); 
#endif

#ifdef SRV_tKeyAb
    StructuredBuffer<BoneType> tKeyAb : register(t2); 
#endif

#ifdef SRV_tHierarchyTable
    StructuredBuffer<uint> tHierarchyTable : register(t3); 
#endif

#ifdef SRV_tSkinBoneWorld
    StructuredBuffer<rowMatrix> tSkinBoneWorld : register(t4); 
#endif

#ifdef SRV_tSkinInvBind
    StructuredBuffer<rowMatrix> tSkinInvBind : register(t5); 
#endif

#ifdef SRV_tKeyBa
    StructuredBuffer<BoneType> tKeyBa : register(t6); 
#endif

#ifdef SRV_tKeyBb
    StructuredBuffer<BoneType> tKeyBb : register(t7); 
#endif

// ----------------------------------------------------
// ux - u is  unordered access views (UAV), x - slot 
//      max count == 8
// ----------------------------------------------------

//#ifdef UAV_xxxx
//    RWStructuredBuffer<BoneType> uTestOut : register(u0); 
//#endif

#ifdef UAV_uBoneWorldOut
    RWStructuredBuffer<rowMatrix> uBoneWorldOut : register(u1); 
#endif

#ifdef UAV_uMixerOutAx
    RWStructuredBuffer<BoneType> uMixerOutAx : register(u2); 
#endif

#ifdef UAV_uMixerOutBx
    RWStructuredBuffer<BoneType> uMixerOutBx : register(u3); 
#endif

#ifdef UAV_uMixerOutCx
    RWStructuredBuffer<BoneType> uMixerOutCx : register(u4); 
#endif

// --------------------------------------------------
// bx - b is CS constant buffer, x - slot
//      in compiled shader its displayed as cbX
//      max count == 15
// --------------------------------------------------

//#ifdef CBV_xxxx
//    cbuffer AA0 : register(b0) 
//    {
//        MixerData xxxx;
//    };
//#endif

#ifdef CBV_csWorld
    cbuffer AA1 : register(b1) 
    {
        WorldData csWorld;
    };
#endif

#ifdef CBV_csMixerA
    cbuffer AA2 : register(b2) 
    {
        MixerData csMixerA;
    };
#endif

#ifdef CBV_csMixerB
    cbuffer AA3 : register(b3) 
    {
        MixerData csMixerB;
    };
#endif

#ifdef CBV_csMixerC
    cbuffer AA4 : register(b4) 
    {
        MixerData csMixerC;
    };
#endif

// --------------------------------------------------
// bx - b is VS constant buffer, x - slot
//      in compiled shader its displayed as cbX
//      max count == 15
// --------------------------------------------------

#ifdef CBV_vsProjectionMatrix
    cbuffer AA0 : register(b0) 
    {
        row_major matrix vsProjectionMatrix;
    }
#endif

#ifdef CBV_vsViewMatrix
    cbuffer AA1 : register(b1) 
    {
        row_major matrix vsViewMatrix;
    }
#endif

#ifdef CBV_vsWorldMatrix
    cbuffer AA2 : register(b2)
    {
        row_major matrix vsWorldMatrix; // this is the pivot
    }
#endif

#ifdef CBV_vsLightColor
    cbuffer AA3 : register(b3) 
    {
        float4 vsLightColor;
    }
#endif

#ifdef CBV_vsLightPos
    cbuffer AA4 : register(b4) 
    {
        float4 vsLightPos;
    }
#endif

//#ifdef CBV_vsSkinInvBind
//    cbuffer AA5 : register(b5) 
//    {
//       row_major matrix vsSkinInvBind[BONE_COUNT_MAX];
//    }
//#endif

//#ifdef CBV_vsSkinBoneWorld
//    cbuffer AA6 : register(b6) 
//    {
//       row_major matrix vsSkinBoneWorld[BONE_COUNT_MAX];
//    }
//#endif


// --------------------------------------------------
// bx - b is PS constant buffer, x - slot
//      in compiled shader its displayed as cbX
//      max count == 15
// --------------------------------------------------


#ifdef CBV_psUVMatrix
    cbuffer AA0 : register(b0) 
    {
        row_major matrix psUVMatrix;
    }
#endif

#ifdef CBV_psColorScale
    cbuffer AA1 : register(b1) 
    {
        float4 psColorScale;
    }
#endif

#ifdef CBV_psColor
    cbuffer AA2 : register(b2) 
    {
        float4 psColor;
    }
#endif



// --- End of File ---