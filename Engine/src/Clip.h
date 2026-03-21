//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "DLink.h"
#include "AnimTime.h"
#include "Skel.h"


namespace Azul
{
	class Mixer;
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

		void Set(Clip::Name clipName,
				 unsigned int numNodes,
				 unsigned int numKeyFrames,
				 AnimFrameBucket *pFrameBucket,
				 Skel::Name skelName);

		void AnimateBones(AnimTime tCurr, Mixer *pMixer);
		virtual void Dump() override;
		virtual void Wash() override;

	private:
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

	public:
		Name         mName;
		size_t       numNodes;
		size_t       numFrames;
		Skel::Name   skelName;
		AnimTime     TotalTime;
		FrameBucket *poHead;
	};
}

#endif

//--- End of File ---
