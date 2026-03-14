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

#include "ShaderMappings.h"
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


	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *pGraphicsObject, Skeleton *_pSkeleton, JointTable* _pJointTable)
		: GameObjectControlled(pGraphicsObject),
		poBoneWorld{nullptr},
		pSkeleton{_pSkeleton},
		pJointTable{ _pJointTable }
	{
		assert(pGraphicsObject);
		assert(pSkeleton);

		// size_t numBones = pSkeleton->GetNumBones();
		this->poBoneWorld = new Mat4[BONE_COUNT_MAX]();

		for(size_t i = 0; i < BONE_COUNT_MAX; i++)
		{
			poBoneWorld[i].set(Identity);
		}

		this->GetGraphicsObject()->GetMesh()->Initialize_SkinBoneWorldBuffer(BONE_COUNT_MAX * sizeof(Mat4));

	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete[] this->poBoneWorld;
	}


	void GameObjectAnimSkin::privUpdateBoneWorldArray()
	{
			// Update the world array
			// Since its already been updated... thankyou hack man
			// we can just add the world matrix into our array

		for (size_t i = 0; i < pJointTable->GetNumBones(); i++)
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

			Mat4 *p = (Mat4 *)MappedResource.pData;

			Trace::out("\n\n");
			Trace::out("----------------------------------\n");
			for (unsigned int i = 0; i < 8; i++)
			{
				Mat4 *pCompute = &p[i];

				Trace::out("\n");
				Trace::out("[%2d]\n", i);
				this->poBoneWorld[i].Print("cpp");
				pCompute->Print("gpu");

			}

			StateDirectXMan::GetContext()->Unmap(uavBoneResult.poComputeUAVBuffer, 0);
		}
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		// Goal: update the world matrix
		this->privUpdateBoneWorldArray();

	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		AZUL_UNUSED_VAR(i);
		// This object doesn't use index like bones do
		// assert(false); 
	}
}

// --- End of File ---
