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

namespace Azul
{

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

		size_t numBones = pJointTable->GetNumBones();
		if (numBones > BONE_COUNT_MAX)
		{
			numBones = BONE_COUNT_MAX;
		}

		for (size_t i = 0; i < numBones; i++)
		{
			GameObjectControlled* pGameObject = pSkeleton->FindBoneByIndex(pJointTable->poJointTable[i]);
			//assert(pGameObject->index == i);
			this->poBoneWorld[i] = *pGameObject->GetWorld();
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
