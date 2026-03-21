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
#include "HierarchyTableMan.h"

namespace Azul
{
	Anim::Anim(HierarchyTable::Name hierarchyName, Clip::Name clipName)
	{
		HierarchyTable* pHierarchyTable = HierarchyTableMan::Find(hierarchyName);
		assert(pHierarchyTable);

		this->poSkeleton = new Skeleton(clipName);
		assert(this->poSkeleton);

		// Setup Compute shaders data
		this->poMixer = new Mixer(this->poSkeleton->GetClip());
		assert(this->poMixer);

		this->poWorldCompute = new WorldCompute(this->poMixer, pHierarchyTable);
		assert(this->poWorldCompute);

	}

	Anim::~Anim()
	{
		delete this->poSkeleton;
		this->poSkeleton = nullptr;

		delete this->poMixer;
		this->poMixer = nullptr;

		delete this->poWorldCompute;
		this->poWorldCompute = nullptr;

	}

	AnimTime Anim::FindMaxTime()
	{
		return this->privGetClip()->GetTotalTime();
	}

	void Anim::Animate(AnimTime tCurr)
	{
		this->privGetClip()->AnimateBones(tCurr, this->poMixer);
	}

	Mixer* Anim::GetMixer()
	{
		assert(this->poMixer);
		return this->poMixer;
	}

	WorldCompute* Anim::GetWorldCompute()
	{
		assert(this->poWorldCompute);
		return this->poWorldCompute;
	}

	Clip* Anim::privGetClip()
	{
		assert(this->poSkeleton);
		Clip* pClip = this->poSkeleton->GetClip();
		assert(pClip);
		return pClip;
	}

	void Anim::SetClip(Clip::Name clipName)
	{
		assert(this->poSkeleton);
		this->poSkeleton->SetClip(clipName);
	}

}

//--- End of File ----
