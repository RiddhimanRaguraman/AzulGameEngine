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
	Anim::Anim(Skeleton *_pSkeleton, Mixer *_pMixer)
		: poSkeleton{ _pSkeleton },
		pClip{ this->poSkeleton->GetClip() },
		pMixer{ _pMixer }
	{
		assert(poSkeleton);
		assert(pClip);
		assert(pMixer);
	}

	Anim::~Anim()
	{
		delete this->poSkeleton;
		this->poSkeleton = nullptr;
	}

	AnimTime Anim::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}
		
	void Anim::Animate(AnimTime tCurr)
	{
		assert(pClip);
		this->pClip->AnimateBones(tCurr, this->pMixer);
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
}

//--- End of File ----
