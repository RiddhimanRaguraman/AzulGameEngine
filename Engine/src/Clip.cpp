//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "AnimTime.h"
#include "Bone.h"
#include "Mixer.h"
#include "StringThis.h"

namespace Azul
{
	Clip::Clip()
		: DLink(),
		mName(Clip::Name::Not_Initialized),
		numBones(0),
		numFrames(0),
		skelName(Skel::Name::Not_Initialized),
		TotalTime(AnimTime::Duration::ZERO),
		poHead(nullptr)
	{

	}

	Clip::~Clip()
	{
		delete[] this->poHead;
	}

	size_t Clip::GetNumBones()
	{
		return this->numBones;
	}

	Skel::Name Clip::GetSkelName()
	{
		return this->skelName;
	}

	void Clip::SetClipName(Name clipName)
	{
		this->mName = clipName;
	}



	void Clip::Dump()
	{
		Trace::out("      Clip(%p)\n", this);
		Trace::out("      Name: %s \n", StringMe(this->mName));

		DLink::Dump();
	}

	void Clip::Wash()
	{
		this->mName = Clip::Name::Not_Initialized;
		this->numBones = 0;
		this->numFrames = 0;
		this->skelName = Skel::Name::Not_Initialized;
		this->TotalTime = AnimTime(AnimTime::Duration::ZERO);
		this->poHead = nullptr;
	}

	void Clip::AnimateBones(AnimTime tCurr, Bone *pResult)
	{
		// First one 
		FrameBucket *pTmp = this->poHead;

		// Find which key frames
		while(tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket *pA = pTmp->prevBucket;
		FrameBucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend(pResult, pA->poBone, pB->poBone, tS, (int)this->numBones);

	}

	void Clip::Set(Clip::Name _clipName,
				   unsigned int _numBones,
				   unsigned int _numKeyFrames,
				   AnimFrameBucket *pFrameBucketEntry,
				   Skel::Name _skelName)
	{
		this->mName = _clipName;
		this->numBones = _numBones;
		this->numFrames = _numKeyFrames;
		this->skelName = _skelName;

		// create the array of FrameBuckets
		this->poHead = new FrameBucket[this->numFrames]();

		FrameBucket *pFirstFrame = this->poHead;
		FrameBucket *pCurrFrame = pFirstFrame;
		FrameBucket *pLastFrame = this->poHead + this->numFrames - 1;

		// now add Bones with data in each bucket
		for(size_t i = 0; i < (size_t)this->numFrames; i++)
		{
			// Fix pointers... TODO - make array base completely
			pCurrFrame->nextBucket = (pCurrFrame + 1);
			pCurrFrame->prevBucket = (pCurrFrame - 1);
			pCurrFrame++;

			// Create Bones
			poHead[i].poBone = new Bone[this->numBones]();

			// Fill the Bones
			Bone *pBone = poHead[i].poBone;
			AnimBone *pEntry = pFrameBucketEntry[i].poBone;

			for(size_t j = 0; j < (size_t)this->numBones; j++)
			{
				pBone[j].S.set(pEntry[j].S.x, pEntry[j].S.y, pEntry[j].S.z);
				pBone[j].Q.set(pEntry[j].Q.qx, pEntry[j].Q.qy, pEntry[j].Q.qz, pEntry[j].Q.qw);
				pBone[j].T.set(pEntry[j].T.x, pEntry[j].T.y, pEntry[j].T.z);
			}

			// Critical - keep time in AnimTime
			poHead[i].KeyTime = pFrameBucketEntry[i].keyTimeIndex * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		}

		// fix up the first and last node
		pLastFrame->nextBucket = nullptr;
		pFirstFrame->prevBucket = nullptr;

		this->TotalTime = this->privFindMaxTime();
	}

	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket *pTmp = this->poHead;

		while(pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	AnimTime Clip::privFindMaxTime()
	{
		AnimTime tMax;
		FrameBucket *pTmp = this->poHead;

		while(pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	AnimTime Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

}

// --- End of File ---
