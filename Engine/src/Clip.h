//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "DLink.h"
#include "AnimTime.h"
#include "Skel.h"
#include "HierarchyTable.h"
#include "Mixer.h"
#include "MixerA.h"
#include "MixerB.h"
#include "MixerC.h"

namespace Azul
{
	class FrameBucket;
	class AnimFrameBucket;

	class Clip : public DLink
	{
	public:
		enum Name
		{
			Walk_ChickenBot,
			Run_ChickenBot,
			HitBack_ChickenBot,
			ShotUp_ChickenBot,
			Idle_ChickenBot,
			Walk_DogBot,
			Run_DogBot,
			HitBack_DogBot,
			ShotUp_DogBot,
			Idle_DogBot,
			walk_Spiderbot,
			Mousey_Silly_Dance,
			Mousey_Run,
			Mousey_Gangnam,
			Not_Initialized
		};

	public:
		Clip();
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		virtual ~Clip();

		AnimTime GetTotalTime();
		size_t GetNumNodes();
		Skel::Name GetSkelName();
		
		void SetClipName(Name clipName);
		Clip::Name GetClipName() const;

		HierarchyTable::Name GetHierarchyName() const;

		void Set(Clip::Name clipName,
				 unsigned int numNodes,
				 unsigned int numKeyFrames,
				 AnimFrameBucket *pFrameBucket,
				 Skel::Name skelName,
	   			 HierarchyTable::Name hierarchyName);

		void AnimateBones(AnimTime tCurr, MixerA *pMixer);
		void AnimateBones(AnimTime tCurr, MixerB* pMixer);
		void AnimateBones(AnimTime tCurr, MixerC* pMixer);

		virtual void Dump() override;
		virtual void Wash() override;

	private:
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

	public:
		Clip::Name           mClipName;
		size_t               numNodes;
		size_t               numFrames;
		Skel::Name           mSkelName;
		HierarchyTable::Name mHierarchyName;
		AnimTime             TotalTime;
		FrameBucket* poHead;
	};
}

#endif

//--- End of File ---
