//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "ShaderMappings.h"
#include "Mesh.h"
#include "Anim.h"
#include "GameObjectAnimSkeleton.h"

#include "ShaderObjectNodeMan.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{

	extern Mat4 HackWorld[];
	extern Bone HackLocalBone[];

	struct WorldConstant
	{
		unsigned int hierarchyDepth;
		unsigned int numBones;
		int pad0;
		int pad1;
	};


	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *_pGraphicsObject, Skeleton *_pSkeleton, JointTable* _pJointTable)
		: GameObjectControlled(_pGraphicsObject),
		poBoneWorld{nullptr},
		pSkeleton{_pSkeleton},
		pJointTable{ _pJointTable }
	{
		assert(_pGraphicsObject);
		assert(this->pSkeleton);

		size_t numNodes = pSkeleton->GetNumNodes();
		this->poBoneWorld = new Mat4[numNodes]();
		for (size_t i = 0; i < numNodes; i++)
		{
			poBoneWorld[i].set(Identity);
		}
		assert(this->poBoneWorld);

		this->poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		this->poScale = new Vec3(1.0f, 1.0f, 1.0f);
		this->poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);

		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);

		this->delta_x = 0.0f;
		this->delta_y = 0.0f;
		this->delta_z = 0.0f;

		this->cur_rot_x = 0.0f;
		this->cur_rot_y = 0.0f;
		this->cur_rot_z = 0.0f;

		this->GetGraphicsObject()->GetMesh()->Initialize_SkinBoneWorldBuffer(this->pJointTable->GetNumJoints() * sizeof(Mat4));// BONE_COUNT_MAX * sizeof(Mat4));

	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete[] this->poBoneWorld;
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->pSkeleton;
	}


	void GameObjectAnimSkin::privUpdateBoneWorldArray()
	{
			// Update the world array
			// Since its already been updated... thankyou hack man
			// we can just add the world matrix into our array

		for (size_t i = 0; i < pJointTable->GetNumJoints(); i++)
		{
			GameObjectControlled* pGameObject = pSkeleton->FindBoneByIndex(pJointTable->poJointTable[i]);
			//assert(pGameObject->index == i);
			this->poBoneWorld[i] = *pGameObject->GetWorld();
		}

		// ------------------------------------------------
		//  Compute Shader Test
		// ------------------------------------------------
		{
			HierarchyTable *pHierarchyTable = HierarchyTableMan::Find(HierarchyTable::Name::ChickenBot);
			assert(pHierarchyTable);

			ShaderObject *pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::WorldCompute);
			pShaderObj->ActivateShader();

			BufferUAV_cs uavBoneLocal(15, sizeof(Bone));
			uavBoneLocal.Transfer(HackLocalBone);
			uavBoneLocal.BindCompute(UnorderedAccessBufferSlot::MixerABOut);

			BufferSRV_cs srvHierarchyTable(pHierarchyTable->GetDepth() * pHierarchyTable->GetNumBones(), sizeof(unsigned int));
			srvHierarchyTable.Transfer(pHierarchyTable->poHierarchyTable);
			srvHierarchyTable.BindCompute(ShaderResourceBufferSlot::HierarchyTable);

			BufferUAV_cs uavBoneResult(8, sizeof(Mat4));
			uavBoneResult.Transfer(HackWorld);
			uavBoneResult.BindCompute(UnorderedAccessBufferSlot::BoneWorldOut);

			BufferCBV_cs cbvWorld(sizeof(WorldConstant));
			WorldConstant worldData;
			worldData.hierarchyDepth = (unsigned int)pHierarchyTable->GetDepth();
			worldData.numBones = (unsigned int)pHierarchyTable->GetNumBones();
			cbvWorld.Transfer(&worldData);
			cbvWorld.BindCompute(ConstantCSBufferSlot::csWorld);

			// Dispatch
			StateDirectXMan::GetContext()->Dispatch(8, 1, 1);


			// Block Waiting for stages to complete
			D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };

			HRESULT hr;
			hr = StateDirectXMan::GetContext()->Map(uavBoneResult.poComputeUAVBuffer,
				0,
				D3D11_MAP_READ,
				0,
				&MappedResource);
			assert(SUCCEEDED(hr));
			assert(MappedResource.pData);

			/*Mat4 *p = (Mat4 *)MappedResource.pData;

			Trace::out("\n\n");
			Trace::out("----------------------------------\n");
			for (unsigned int i = 0; i < 8; i++)
			{
				Mat4 *pCompute = &p[i];

				Trace::out("\n");
				Trace::out("[%2d]\n", i);
				this->poBoneWorld[i].Print("cpp");
				pCompute->Print("gpu");

			}*/

			StateDirectXMan::GetContext()->Unmap(uavBoneResult.poComputeUAVBuffer, 0);
		}
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		this->privUpdateBoneWorldArray();

		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		Trans T(*this->poTrans);
		Scale S(*this->poScale);
		Quat  Q(*this->poQuat);

		cur_rot_x += delta_x;
		cur_rot_y += delta_y;
		cur_rot_z += delta_z;

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		// world matrix
		*this->poWorld = S * Q * Rx * Ry * Rz * T;
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnimSkin::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void GameObjectAnimSkin::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnimSkin::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		AZUL_UNUSED_VAR(i);
		// This object doesn't use index like bones do
		// assert(false); 
	}
}

// --- End of File ---
