//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Skeleton.h"
#include "SkelMan.h"
#include "ClipMan.h"
#include "Prefab_Pivot.h"


namespace Azul
{

	Skeleton::Skeleton(Clip::Name _ClipName)
		: mNumNodes(0),
		poBoneResult(nullptr),
		pSkel(nullptr),
		pClip(nullptr)
	{
		assert(_ClipName != Clip::Name::Not_Initialized);

		this->pClip = ClipMan::Find(_ClipName);
		assert(this->pClip);

		this->mNumNodes = (int)this->pClip->GetNumNodes();

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


	Clip *Skeleton::GetClip()
	{
		return this->pClip;
	}

	Bone *Skeleton::GetBoneResult()
	{
		return this->poBoneResult;
	}


	int Skeleton::GetNumNodes() const
	{
		return this->mNumNodes;
	}
}

// --- End of File ---
