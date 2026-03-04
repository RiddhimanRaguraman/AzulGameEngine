//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "Clip.h"
#include "ClipMan.h"

namespace Azul
{
	Anim::Anim(Clip::Name clipName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable)
		: pClip{ nullptr },
		poBoneResult(nullptr),
		poSkeleton(nullptr),
		numBones(0)
	{

		this->pClip = ClipMan::Find(clipName);
		assert(this->pClip);
		this->numBones = (unsigned int)pClip->GetNumBones();

		this->poBoneResult = new Bone[this->numBones]();
		assert(poBoneResult);

		this->poSkeleton = new Skeleton(this->poBoneResult, numBones, pClip->GetSkelName(), texName, meshName, pJointTable);
		assert(this->poSkeleton);
	}

    Anim::~Anim()
	{
		delete[] this->poBoneResult;
		delete   this->poSkeleton;
	}

    AnimTime Anim::FindMaxTime()
    {
        assert(pClip);
        return this->pClip->GetTotalTime();
    }

    void Anim::Animate(AnimTime tCurr)
    {
		// if the Clip to set externly
        if (!this->pClip)
        {
            return;
        }
        this->pClip->AnimateBones(tCurr, this->poBoneResult);
    }

	void Anim::SetClip(Clip::Name clipName)
	{
		this->pClip = ClipMan::Find(clipName);
		assert(this->pClip);
		this->numBones = (unsigned int)this->pClip->GetNumBones();
	}

    Clip *Anim::GetClip()
    {
        return this->pClip;
    }

    void Anim::SetPivotScale(float sx, float sy, float sz)
    {
        this->poSkeleton->SetPivotScale(sx, sy, sz);
    }

    void Anim::SetUniformPivotScale(float s)
    {
        this->poSkeleton->SetUniformPivotScale(s);

    }

    void Anim::SetPivotTrans(float x, float y, float z)
    {
        this->poSkeleton->SetPivotTrans(x, y, z);
    }

	void Anim::SetPivotRotX(float angle)
	{
		this->poSkeleton->SetPivotRotX(angle);
	}
	void Anim::SetPivotRotY(float angle)
	{
		this->poSkeleton->SetPivotRotY(angle);
		
	}
	void Anim::SetPivotRotZ(float angle)
	{
		this->poSkeleton->SetPivotRotZ(angle);
		
	}
	void Anim::SetPivotTotalRot(const Rot3 mode, float x, float y, float z)
	{
		this->poSkeleton->SetPivotTotalRot(mode, x, y, z);
		
	}

}

//--- End of File ----
