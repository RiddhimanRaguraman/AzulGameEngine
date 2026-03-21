//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_H
#define MIXER_H

#include "AnimTimer.h"
#include "BufferCBV_cs.h"
#include "BufferUAV_cs.h"

namespace Azul
{
	class Clip;
	class BufferSRV_cs;

	// structure for Constant Compute buffer needs to be multiple of 16 bytes
	struct MixerConstant
	{
		float ts;
		unsigned int numNodes;
		int pad0;
		int pad1;
	};

	class Mixer
	{
	public:
		Mixer(Clip *pClip);

		Mixer() = delete;
		Mixer(const Mixer &) = delete;
		Mixer &operator = (const Mixer &) = delete;
		virtual ~Mixer();

		BufferCBV_cs *GetMixerConstBuff();
		BufferUAV_cs *GetMixerResult();
		void *GetRawConstBuffer();
		size_t GetNumNodes();

	public:
		BufferSRV_cs *pKeyA;
		BufferSRV_cs *pKeyB;
		BufferUAV_cs  mMixerResult;
		BufferCBV_cs  mMixerConstBuffer;
		float		  tS;
		size_t        numNodes;
	};
}

#endif

// --- End of File ---
