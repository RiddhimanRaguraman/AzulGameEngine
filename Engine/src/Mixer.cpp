//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Mixer.h"
#include "MathApp.h"
#include "ShaderObjectNodeMan.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"
#include "Clip.h"
#include "Bone.h"

namespace Azul
{
	Mixer::Mixer(Clip *pClip)
		:pKeyA{ nullptr },
		pKeyB{ nullptr },
		mMixerResult{ pClip->GetNumNodes(), sizeof(Bone) },
		mMixerConstBuffer{ sizeof(MixerConstant) },
		tS{ 0 },
		numNodes{ pClip->GetNumNodes() }
	{
		assert(pClip);
	}

	Mixer::~Mixer()
	{
		// nothing since the buffers are objects
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
		static MixerConstant tmp{ 0 };
		tmp.numNodes = this->numNodes;
		tmp.ts = this->tS;

		return &tmp;
	}

	size_t Mixer::GetNumNodes()
	{
		return this->numNodes;
	}

}

// --- End of File ---