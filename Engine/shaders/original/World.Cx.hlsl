//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

struct BoneType
{
    float4 t;
    float4 q;
    float4 s;
};

struct rowMatrix
{
    row_major matrix m;
};

// Function
row_major matrix Bone2Matrix(BoneType bone);
row_major matrix TransMatrix(float4 trans);
row_major matrix ScaleMatrix(float4 scale);
row_major matrix RotMatrix(float4 quat);

// ---------------------------------------------
// tx - t is shader resource views (SRV), x - slot
// ---------------------------------------------
                                                                  
StructuredBuffer<uint> HierarchyTable : register(t3); // slot 3  (ShaderResourceBufferSlot::HierarchyTable)

// ------------------------------------------
// bx - b is constant buffer, x - slot
// ------------------------------------------

cbuffer AA : register(b1) // slot 1  (ConstantCSBufferSlot::csWorld)
{
    uint hierarchyDepth;
    uint numBones;
};

// ----------------------------------------------------
// ux - u is  unordered access views (UAV), x - slot 
// ----------------------------------------------------

    RWStructuredBuffer<BoneType> MixerABOut : register(u0); // slot 0  UnorderedAccessBufferSlot::MixerABOut
    RWStructuredBuffer<rowMatrix> BoneWorldOut : register(u1); // slot 1  UnorderedAccessBufferSlot::BoneWorldOut


[numthreads(1, 1, 1)]
void main(uint3 dtID : SV_DispatchThreadID)
{
    uint boneIndex = dtID.x;

    if (dtID.x < numBones)
    {
		// identity
        row_major matrix tmp =
        {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 0, 1, 0 },
            { 0, 0, 0, 1 }
        };

        for (uint i = 0; i < hierarchyDepth; i++)
        {
		    //  parentIndex <- Table(Row + offset)
            int parentIndex = HierarchyTable[(boneIndex * hierarchyDepth) + i];

            if (parentIndex != -1)
            {
			    // concatenate
                tmp = mul(Bone2Matrix(MixerABOut[parentIndex]), tmp);
            }
        }

        BoneWorldOut[boneIndex].m = tmp;
    }
}

row_major matrix Bone2Matrix(BoneType bone)
{
    row_major matrix mTrans = TransMatrix(bone.t);
    row_major matrix mScale = ScaleMatrix(bone.s);
    row_major matrix mRot = RotMatrix(bone.q);

    row_major matrix result;

    result = mul(mul(mScale, mRot), mTrans);

    return result;
}

row_major matrix TransMatrix(float4 trans)
{
    row_major matrix result;
    
    // don't know why its col major
    result[0] = float4(1, 0, 0, 0);
    result[1] = float4(0, 1, 0, 0);
    result[2] = float4(0, 0, 1, 0);
    result[3] = float4(trans.x, trans.y, trans.z, 1);
    return result;
}

row_major matrix ScaleMatrix(float4 scale)
{
    row_major matrix result;
    result[0] = float4(scale.x, 0, 0, 0);
    result[1] = float4(0, scale.y, 0, 0);
    result[2] = float4(0, 0, scale.z, 0);
    result[3] = float4(0, 0, 0, 1);
    return result;
}

row_major matrix RotMatrix(float4 q)
{
    row_major matrix result;

    float x2, y2, z2;

    x2 = 2 * q.x;
    y2 = 2 * q.y;
    z2 = 2 * q.z;

    float xx, xy, xz;
    float yy, yz, zz;
    float wx, wy, wz;

    xx = q.x * x2;
    xy = q.x * y2;
    xz = q.x * z2;

    yy = q.y * y2;
    yz = q.y * z2;
    zz = q.z * z2;

    wx = q.w * x2;
    wy = q.w * y2;
    wz = q.w * z2;

    result[0] = float4(1.0 - (yy + zz),      xy + wz,        xz - wy, 0);
    result[1] = float4(        xy - wz, 1 - (xx + zz),       yz + wx, 0);
    result[2] = float4(        xz + wy,       yz - wx, 1 - (xx + yy), 0);
    result[3] = float4(              0,             0,             0, 1);

    return result;
}

// --- End of File ---
