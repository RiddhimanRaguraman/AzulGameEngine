//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshProto.h"
#include "MathEngine.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"
#include "Engine.h"
#include "File.h"
#include "meshData.h"

namespace Azul
{
	MeshProto::MeshProto(const char* const pMeshFileName)
		: Mesh(0, 0),
		VBVBuffer_pos(),
		VBVBuffer_color(),
		VBVBuffer_tex(),
		VBVBuffer_norm(),
		VBVBuffer_weight(),
		VBVBuffer_joint(),
		IBVBuffer(),
		CBVBuffer_InvBind(),
		CBVBuff_SkinBoneWorld()
	{
		// future proofing it for a file
		assert(pMeshFileName);

		unsigned char* poBuff;
		unsigned int numBytes;

		File::Error ferror;
		ferror = File::GetFileAsBuffer(pMeshFileName, poBuff, numBytes);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		// deserialize the data --> mB
		std::string strIn((const char*)poBuff, numBytes);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		memcpy_s(this->pModelName, Mesh::MESH_MODEL_NAME_SIZE, mB.pMeshName, strlen(mB.pMeshName) + 1);

		// Now the model is mB...
		// move it into the Mesh

		// Do I know this format?
		assert(strcmp(mB.pVersion, "2.0.0") == 0);

		// Using the hdr, allocate the space for the buffers
		this->numVerts = (int)mB.vbo_vert.count;
		this->numIndices = (int)mB.triCount * 3;

		assert(this->numIndices > 0);
		assert(this->numVerts > 0);

		// Load the verts data: ---------------------------------------------------------

		// ------------------------------------------------
		// Create an initialize the vertex buffer - pos
		// ------------------------------------------------
		if (mB.vbo_vert.enabled)
		{
			assert(mB.vbo_vert.poData);
			assert(mB.vbo_vert.dataSize > 0);
			assert(mB.vbo_vert.vboType == vboData::VBO_TYPE::VEC3);
			assert(mB.vbo_vert.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_pos.Initialize(mB.vbo_vert.dataSize, mB.vbo_vert.stride, mB.vbo_vert.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - color
		// ------------------------------------------------
		if (mB.vbo_color.enabled)
		{
			assert(mB.vbo_color.poData);
			assert(mB.vbo_color.dataSize > 0);
			assert(mB.vbo_color.vboType == vboData::VBO_TYPE::VEC4);
			assert(mB.vbo_color.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_color.Initialize(mB.vbo_color.dataSize, mB.vbo_color.stride, mB.vbo_color.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - uv
		// ------------------------------------------------
		if (mB.vbo_uv.enabled)
		{
			assert(mB.vbo_uv.poData);
			assert(mB.vbo_uv.dataSize > 0);
			assert(mB.vbo_uv.vboType == vboData::VBO_TYPE::VEC2);
			assert(mB.vbo_uv.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_tex.Initialize(mB.vbo_uv.dataSize, mB.vbo_uv.stride, mB.vbo_uv.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - norms
		// ------------------------------------------------
		if (mB.vbo_norm.enabled)
		{
			assert(mB.vbo_norm.poData);
			assert(mB.vbo_norm.dataSize > 0);
			assert(mB.vbo_norm.vboType == vboData::VBO_TYPE::VEC3);
			assert(mB.vbo_norm.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_norm.Initialize(mB.vbo_norm.dataSize, mB.vbo_norm.stride, mB.vbo_norm.poData);
		}
		// ------------------------------------------------
		// Create and initialize the index buffer.
		// ------------------------------------------------
		if (mB.vbo_index.enabled)
		{
			assert(mB.vbo_index.vboType == vboData::VBO_TYPE::SCALAR);
			assert(mB.vbo_index.componentType != vboData::VBO_COMPONENT::DOUBLE
				&& mB.vbo_index.componentType != vboData::VBO_COMPONENT::FLOAT);

			assert(mB.vbo_index.dataSize > 0);
			assert(mB.vbo_index.poData);


			//Vec3ui *pTmp = (Vec3ui *)mB.vbo_index.poData;
			//for(size_t i = 0; i < mB.vbo_index.count ; i++)
			//{
			//	Trace::out("[%d] %d %d %d \n", i, pTmp[i].x, pTmp[i].y, pTmp[i].z);
			//}

			IBVBuffer.Initialize(mB.vbo_index.dataSize, mB.vbo_index.poData);
		}

		// ------------------------------------------------
		// Create an initialize the vertex buffer - weights
		// ------------------------------------------------
		if (mB.vbo_weights.enabled)
		{
			assert(mB.vbo_weights.poData);
			assert(mB.vbo_weights.dataSize > 0);
			assert(mB.vbo_weights.vboType == vboData::VBO_TYPE::VEC4);
			assert(mB.vbo_weights.componentType == vboData::VBO_COMPONENT::FLOAT);

			numBytes = mB.vbo_weights.dataSize;
			//void *pBuff = mB.vbo_weights.poData;

			//Vec4f *pTmp = (Vec4f *)pBuff;
			//for(size_t i = 0; i < mB.vbo_joints.count; i++)
			//{
			//	Trace::out("[%d]  %f %f %f %f \n", i,
			//			   pTmp[i].x, pTmp[i].y, pTmp[i].z, pTmp[i].w);
			//}

			VBVBuffer_weight.Initialize(mB.vbo_weights.dataSize, mB.vbo_weights.stride, mB.vbo_weights.poData);
		}

		// ------------------------------------------------
		// Create an initialize the vertex buffer - joints
		// ------------------------------------------------
		if (mB.vbo_joints.enabled)
		{
			assert(mB.vbo_joints.poData);
			assert(mB.vbo_joints.dataSize > 0);
			assert(mB.vbo_joints.vboType == vboData::VBO_TYPE::VEC4);
			assert(mB.vbo_joints.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT);

			numBytes = mB.vbo_joints.dataSize;

			//Vec4ui *pTmp = (Vec4ui *)mB.vbo_joints.poData;
			//for(size_t i = 0; i < mB.vbo_joints.count; i++)
			//{
			//	Trace::out("[%d]  %d %d %d %d \n", i,
			//			   pTmp[i].x, pTmp[i].y, pTmp[i].z, pTmp[i].w);
			//}

			VBVBuffer_joint.Initialize(mB.vbo_joints.dataSize, mB.vbo_joints.stride, mB.vbo_joints.poData);

			// joint table is initialize (count is the same)		
			//this->CBVBuff_SkinBoneWorld.Initialize(8 * sizeof(Mat4));

		}

		// Inverse Bind Matrix Array
		if (mB.vbo_invBind.enabled)
		{
			assert(mB.vbo_invBind.vboType == vboData::VBO_TYPE::MAT4);
			assert(mB.vbo_invBind.componentType == vboData::VBO_COMPONENT::FLOAT);
			assert(mB.vbo_invBind.dataSize > 0);
			assert(mB.vbo_invBind.poData);

			numBytes = mB.vbo_invBind.dataSize;

			// transfer to local copy
			//Mat4 *pMat = (Mat4 *)mB.vbo_invBind.poData;
			//for(size_t i = 0; i < mB.vbo_invBind.count; i++)
			//{
			//	Trace::out("i:%d ", i);
			//	pMat[i].Print("InvBind");
			//}

			CBVBuffer_InvBind.Initialize(BONE_COUNT_MAX * sizeof(Mat4));
			
			// Create a temp buffer that is the full size
			Mat4* pTemp = new Mat4[BONE_COUNT_MAX]();
			for (unsigned int i = 0; i < BONE_COUNT_MAX; i++)
			{
				pTemp[i].set(Identity);
			}

			// Copy the data over
			size_t copySize = mB.vbo_invBind.dataSize;
			if (copySize > BONE_COUNT_MAX * sizeof(Mat4))
			{
				copySize = BONE_COUNT_MAX * sizeof(Mat4);
			}

			memcpy_s(pTemp, BONE_COUNT_MAX * sizeof(Mat4), mB.vbo_invBind.poData, copySize);

			CBVBuffer_InvBind.Transfer(pTemp);

			delete[] pTemp;
		}
	}

	void MeshProto::ActivateConstantBuffers()
	{
		// Skin extra
		this->CBVBuffer_InvBind.SetActive(ConstantVSBufferSlot::vsSkinInvBind);
		this->CBVBuff_SkinBoneWorld.SetActive(ConstantVSBufferSlot::vsSkinBoneWorld);
	}

	void MeshProto::Transfer_SkinBoneWorldBuffer(Mat4* p)
	{
		assert(p);
		CBVBuff_SkinBoneWorld.Transfer(p);
	}

	void MeshProto::Initialize_SkinBoneWorldBuffer(size_t _structSize)
	{
		AZUL_UNUSED_VAR(_structSize);
		this->CBVBuff_SkinBoneWorld.Initialize(BONE_COUNT_MAX * sizeof(Mat4));
	}

	void MeshProto::ActivateMesh()
	{
		// ---------------------------------------------
		//    Set (point to ) vertex: position 
		//    Set (point to ) vertex: color
		//    Set (point to ) vertex: texCoord
		//    Set (point ot ) vertex: norm
		// ---------------------------------------------
		this->VBVBuffer_pos.SetActive(VertexSlot::Position);
		this->VBVBuffer_color.SetActive(VertexSlot::Color);
		this->VBVBuffer_tex.SetActive(VertexSlot::TexCoord);
		this->VBVBuffer_norm.SetActive(VertexSlot::Norm);

		// Skin extra
		this->VBVBuffer_joint.SetActive(VertexSlot::Joints);
		this->VBVBuffer_weight.SetActive(VertexSlot::Weights);
	}

	void MeshProto::RenderIndexBuffer()
	{
		// ---------------------------------------------
		//    Set (point to ) Index buffer 
		//    Render configuration: Triangles
		// ---------------------------------------------
		this->IBVBuffer.SetActive();
		StateDirectXMan::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//--------------------------------------------------------
		// RENDER - using index data
		//--------------------------------------------------------
		StateDirectXMan::GetContext()->DrawIndexed(numIndices, 0, 0);
	}

	MeshProto::~MeshProto()
	{

	}
}

// --- End of File ---