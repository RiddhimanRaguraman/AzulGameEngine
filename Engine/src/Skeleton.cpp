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

namespace Azul
{

	Skeleton::Skeleton(Bone* pBone, unsigned int _numBones, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable)
		: pFirstBone(nullptr),
		numBones(_numBones)
	{
		assert(pBone);
		this->privSetAnimationHierarchy(pBone, skelName, texName, meshName, pJointTable);
		assert(pFirstBone);
	}

	Skeleton::~Skeleton()
	{

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

	void Skeleton::privSetAnimationHierarchy(Bone* pBoneResult, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable)
	{
		assert(pBoneResult);

		Skel* pSkel = SkelMan::Find(skelName);
		assert(pSkel);

		// GraphicsObject for a specific instance
		GraphicsObject* pGraphicsObj;

		// Create GameObject
		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);

		//---------------------------------------
		// ChickenBot
		//---------------------------------------

		// Add A Dummy pivot object
		pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH, ShaderObject::Name::NullShader);
		GameObjectRigidBody* pPivotObj = new GameObjectRigidBody(pGraphicsObj);
		pPivotObj->SetName("Pivot");
		pPivotObj->DrawDisable();
		pPivotObj->SetPrefab(new Prefab_Pivot());
		this->pPivot = pPivotObj;
		// Add Bones in Hierarchy
		GameObjectMan::Add(pPivotObj, GameObjectMan::GetRoot());

		// Animation
		GameObjectControlled* pGameObj;

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin;
		Vec3 LightColor(1.5f, 1.50f, 1.50f);
		Vec3 LightPos(1.0f, 1.0f, 100.0f);
		GraphicsObject_SkinLightTexture* pGraphicsSkin;


		pGraphicsSkin = new GraphicsObject_SkinLightTexture(meshName, // get from input later 
															ShaderObject::Name::SkinFlatTexture,
															texName, LightColor, LightPos);

		pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, this, pJointTable);

		// Glue the bone array together - Hack
		pGraphicsSkin->SetBoneWorld(pGameSkin->poBoneWorld);

		GameObjectMan::Add(pGameSkin, pPivot);
		pGameSkin->SetName("Mousey");

		
		// Root animation is treated differently
		for (int i = 0; i < 1; i++)
		{
			pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH,
				ShaderObject::Name::NullShader);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(pSkel->poTableArray[i].index);
			pGameObj->SetName(pSkel->poTableArray[i].name);

			GameObjectMan::Add(pGameObj, pPivot);
			this->pFirstBone = pGameObj;
		}

		for (int i = 1; i < this->numBones; i++)
		{
			pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH,
				ShaderObject::Name::NullShader);
			pGameObj = new GameObjectAnimSkeleton(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(pSkel->poTableArray[i].index);
			pGameObj->SetName(pSkel->poTableArray[i].name);

			GameObjectControlled* pParent = this->FindBoneByIndex(pSkel->poTableArray[i].parentIndex);
			assert(pParent);
			GameObjectMan::Add(pGameObj, pParent);
		}

	}
	
	int Skeleton::GetNumBones() const
	{
		return this->numBones;
	}

	void Skeleton::SetPivotScale(float sx, float sy, float sz)
	{
		if (this->pPivot)
		{
			this->pPivot->SetScale(sx, sy, sz);
		}
	}

	void Skeleton::SetUniformPivotScale(float s)
	{
		if (this->pPivot)
		{
			this->pPivot->SetUniformScale(s);
		}
	}


	void Skeleton::SetPivotTrans(float x, float y, float z)
	{
		if (this->pPivot)
		{
			this->pPivot->SetTrans(x, y, z);
		}
	}

	void Skeleton::SetPivotRotX(float angle)
	{
		if (this->pPivot)
		{
			this->pPivot->SetRotX(angle);
		}
	}
	void Skeleton::SetPivotRotY(float angle)
	{
		if (this->pPivot)
		{
			this->pPivot->SetRotY(angle);
		}
	}
	void Skeleton::SetPivotRotZ(float angle)
	{
		if (this->pPivot)
		{
			this->pPivot->SetRotZ(angle);
		}
	}
	void Skeleton::SetPivotTotalRot(const Rot3 mode, float x, float y, float z)
	{
		if (this->pPivot)
		{
			this->pPivot->SetTotalRot(mode,x,y,z);
		}
	}
}

// --- End of File ---
