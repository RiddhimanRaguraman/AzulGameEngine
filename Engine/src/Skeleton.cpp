//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_SkinLightTexture.h"
#include "GameObjectMan.h"
#include "ShaderObjectNodeMan.h"
#include "GameObjectRigidBody.h"
#include "GraphicsObject_Null.h"
#include "Skeleton.h"
#include "MeshNodeMan.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "TexNodeMan.h"
#include "PCSTreeForwardIterator.h"
#include "Prefab_Pivot.h"
#include "SkelMan.h"
#include "ClipMan.h"

namespace Azul
{

	Skeleton::Skeleton(Clip::Name _ClipName)
		: pFirstBone(nullptr),
		mNumNodes(0),
		poBoneResult(nullptr),
		pSkel(nullptr),
		pClip(nullptr)
	{
		assert(_ClipName != Clip::Name::Not_Initialized);

		this->pClip = ClipMan::Find(_ClipName);
		assert(this->pClip);

		this->mNumNodes = this->pClip->GetNumNodes();

		this->poBoneResult = new Bone[this->mNumNodes]();
		assert(this->poBoneResult);

		this->pSkel = SkelMan::Find(pClip->GetSkelName());
		assert(this->pSkel);
	}

	Skeleton::~Skeleton()
	{
		delete[] this->poBoneResult;
		this->poBoneResult = nullptr;
	}

	void Skeleton::SetClip(Clip::Name clipName)
	{
		assert(clipName != Clip::Name::Not_Initialized);
		Clip *pNewClip = ClipMan::Find(clipName);
		assert(pNewClip);

		assert((int)pNewClip->GetNumNodes() == this->mNumNodes);

		this->pClip = pNewClip;
		this->pSkel = SkelMan::Find(pNewClip->GetSkelName());
		assert(this->pSkel);
	}

	void Skeleton::SetAnimationHierarchy(GameObject *pParent)
	{
		assert(pParent);

		if (this->pFirstBone == nullptr)
		{
			this->privSetAnimationHierarchy(pParent);
			assert(this->pFirstBone);
		}
	}

	Clip *Skeleton::GetClip()
	{
		return this->pClip;
	}

	Bone *Skeleton::GetBoneResult()
	{
		return this->poBoneResult;
	}

	GameObjectControlled* Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	GameObjectControlled* Skeleton::FindBoneByIndex(int index)
	{
		GameObjectControlled* pFound = nullptr;
		GameObjectControlled* pObj = this->GetFirstBone();

		// TODO - add test bed for an interator of a tree with only one node
		if (pObj->index == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode* pNode = pIter.First();
			GameObjectControlled* pGameObj = nullptr;

			// walks the anim node does the pose for everything that
			while (pNode != nullptr)
			{
				assert(pNode);
				// Update the game object
				pGameObj = (GameObjectControlled*)pNode;
				if (pGameObj->index == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(GameObject *pParent)
	{
		assert(pParent);
		assert(this->poBoneResult);
		assert(this->pSkel);;

		// GraphicsObject for a specific instance
		GraphicsObject* pGraphicsObj;

		// Create GameObject
		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);

		// Animation
		GameObjectControlled *pGameObj;
				
		// Root animation is treated differently
		for (int i = 0; i < 1; i++)
		{
			pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH,
				ShaderObject::Name::NullShader);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, this->poBoneResult);
			pGameObj->SetIndex(pSkel->poTableArray[i].index);
			pGameObj->SetName(pSkel->poTableArray[i].name);

			GameObjectMan::Add(pGameObj, pParent);
			this->pFirstBone = pGameObj;
		}

		for (int i = 1; i < this->mNumNodes; i++)
		{
			pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH,
				ShaderObject::Name::NullShader);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, this->poBoneResult);
			pGameObj->SetIndex(pSkel->poTableArray[i].index);
			pGameObj->SetName(pSkel->poTableArray[i].name);

			GameObjectControlled* pParent = this->FindBoneByIndex(pSkel->poTableArray[i].parentIndex);
			assert(pParent);
			GameObjectMan::Add(pGameObj, pParent);
		}

	}
	
	int Skeleton::GetNumNodes() const
	{
		return this->mNumNodes;
	}

	void Skeleton::SetPivotScale(float sx, float sy, float sz)
	{
		AZUL_UNUSED_VAR(sx);
		AZUL_UNUSED_VAR(sy);
		AZUL_UNUSED_VAR(sz);
	}

	void Skeleton::SetUniformPivotScale(float s)
	{
		AZUL_UNUSED_VAR(s);
	}


	void Skeleton::SetPivotTrans(float x, float y, float z)
	{
		AZUL_UNUSED_VAR(x);
		AZUL_UNUSED_VAR(y);
		AZUL_UNUSED_VAR(z);
	}

	void Skeleton::SetPivotRotX(float angle)
	{
		AZUL_UNUSED_VAR(angle);
	}
	void Skeleton::SetPivotRotY(float angle)
	{
		AZUL_UNUSED_VAR(angle);
	}
	void Skeleton::SetPivotRotZ(float angle)
	{
		AZUL_UNUSED_VAR(angle);
	}
	void Skeleton::SetPivotTotalRot(const Rot3 mode, float x, float y, float z)
	{
		AZUL_UNUSED_VAR(mode);
		AZUL_UNUSED_VAR(x);
		AZUL_UNUSED_VAR(y);
		AZUL_UNUSED_VAR(z);
	}
}

// --- End of File ---
