//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "AnimTime.h"
#include "Bone.h"
#include "Mixer.h"
#include "StringThis.h"

#include "FrameBucket.h"
#include "AnimFrameBucket.h"
#include "BufferSRV_cs.h"

namespace Azul
{
	Clip::Clip()
		: DLink(),
		mName(Clip::Name::Not_Initialized),
		numNodes(0),
		numFrames(0),
		skelName(Skel::Name::Not_Initialized),
		TotalTime(AnimTime::Duration::ZERO),
		poHead(nullptr)
	{

	}

	Clip::~Clip()
	{
		delete[] this->poHead;
		this->poHead = nullptr;
	}

	size_t Clip::GetNumNodes()
	{
		return this->numNodes;
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
		this->numNodes = 0;
		this->numFrames = 0;
		this->skelName = Skel::Name::Not_Initialized;
		this->TotalTime = AnimTime(AnimTime::Duration::ZERO);
		this->poHead = nullptr;
	}

	void Clip::AnimateBones(AnimTime tCurr, Mixer *pMixer)
	{
		assert(pMixer);

		// First one 
		FrameBucket *pTmp = this->poHead;
		int count = 0;

		// safety - make sure there are bones to animate
		if (pTmp->nextBucket != nullptr)
		{
		// Find which key frames
		while(tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
			count++;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket *pA = pTmp->prevBucket;
		FrameBucket *pB = pTmp;

		pMixer->pKeyA = pA->poBoneSRV;
		pMixer->pKeyB = pB->poBoneSRV;

		// find the "S" of the time
		pMixer->tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		}
		else
		{
			pMixer->pKeyA = pTmp->poBoneSRV;
			pMixer->pKeyB = pTmp->poBoneSRV;

			// find the "S" of the time
			pMixer->tS = 0.0f;
		}

	}

	void Clip::Set(Clip::Name _clipName,
				   unsigned int _numBones,
				   unsigned int _numKeyFrames,
				   AnimFrameBucket *pFrameBucketEntry,
				   Skel::Name _skelName)
	{
		this->mName = _clipName;
		this->numNodes = _numBones;
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

		// ------------------------------------------
		// Transfer Bone data to SRV Buffer
		// ------------------------------------------
			{
				// make the storage - now its a SRV before it was a BoneArray on CPU
				poHead[i].poBoneSRV = new BufferSRV_cs(this->numNodes, sizeof(Bone));

				// --- Now fill it ---

				// Temp CPU buffer
				Bone *pCPUBone = new Bone[this->numNodes]();

				// Fill it
				Bone *pTmp = &pCPUBone[0];
				AnimBone *pEntry = pFrameBucketEntry[i].poBone;

				for (size_t j = 0; j < this->numNodes; j++)
				{
					pTmp[j].S.set(pEntry[j].S.x, pEntry[j].S.y, pEntry[j].S.z);
					pTmp[j].Q.set(pEntry[j].Q.qx, pEntry[j].Q.qy, pEntry[j].Q.qz, pEntry[j].Q.qw);
					pTmp[j].T.set(pEntry[j].T.x, pEntry[j].T.y, pEntry[j].T.z);
				}

				// pBone is filled so transfer it
				poHead[i].poBoneSRV->Transfer(pCPUBone);

				// Remove tmp buffer
				delete[] pCPUBone;
				pCPUBone = nullptr;
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
