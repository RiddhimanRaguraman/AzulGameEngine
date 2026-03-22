//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Mixer.h"
#include "Bone.h"
#include "Clip.h"

namespace Azul
{
	Mixer::Mixer(Clip *pClip)
		:
		mMixerResult{pClip->GetNumNodes(), sizeof(Bone)},
		mMixerConstBuffer{sizeof(MixerConstant)},
		poMixerConstant{new MixerConstant((unsigned int)pClip->GetNumNodes(),0.0f)}
	{
		assert(pClip);
		assert(poMixerConstant);
	}

	Mixer::~Mixer()
	{
		delete this->poMixerConstant;
		this->poMixerConstant = nullptr;
	}

	BufferUAV_cs *Mixer::GetMixerResult()
	{
		return &this->mMixerResult;
	}

	BufferCBV_cs *Mixer::GetMixerConstBuff()
	{
		return &this->mMixerConstBuffer;
	}

	void *Mixer::GetRawConstBuffer()
	{
		assert(this->poMixerConstant);
		return this->poMixerConstant;
	}

	size_t Mixer::GetNumNodes()
	{
		return this->poMixerConstant->numNodes;
	}

}

// --- End of File ---
