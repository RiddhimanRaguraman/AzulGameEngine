//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Anim.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "MixerA.h"
#include "Clip.h"
#include "ClipMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{
	Anim::Anim(Clip::Name clipName)
		:
		poSkeleton{ new Skeleton(clipName) },
		pClip{ this->poSkeleton->GetClip() },
		mClipName{ clipName }
	{
		assert(this->poSkeleton);
		assert(this->pClip);

		assert(this->mClipName != Clip::Name::Not_Initialized);

	}

	Anim::~Anim()
	{
		delete this->poSkeleton;
		this->poSkeleton = nullptr;

	}

	AnimTime Anim::FindMaxTime()
	{
		return this->GetClip()->GetTotalTime();
	}

	Clip* Anim::GetClip()
	{
		assert(this->pClip);
		return pClip;
	}

	void Anim::SetClip(Clip::Name clipName)
	{
		assert(this->poSkeleton);
		this->poSkeleton->SetClip(clipName);
	}

}

//--- End of File ----
