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
	Anim::Anim(Skeleton *pSkeleton)
		: poSkeleton{ pSkeleton }
	{
		assert(poSkeleton);
	}

	Anim::~Anim()
	{
		this->poSkeleton = nullptr;
	}

	AnimTime Anim::FindMaxTime()
	{
		assert(this->poSkeleton);

		Clip *pClip = this->poSkeleton->GetClip();
		assert(pClip);

		return pClip->GetTotalTime();
	}

	void Anim::Animate(AnimTime tCurr)
	{
		assert(this->poSkeleton);

		Clip *pClip = this->poSkeleton->GetClip();
		assert(pClip);

		pClip->AnimateBones(tCurr, this->poSkeleton->GetBoneResult());
	}

	void Anim::SetClip(Clip::Name clipName)
	{
		assert(this->poSkeleton);
		this->poSkeleton->SetClip(clipName);
	}

    Clip *Anim::GetClip()
    {
		assert(this->poSkeleton);
        return this->poSkeleton->GetClip();
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
