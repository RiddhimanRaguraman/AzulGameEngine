//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_H
#define MIXER_H

#include "BufferCBV_cs.h"
#include "BufferUAV_cs.h"


namespace Azul
{
	class Clip;

	// structure for Constant Compute buffer needs to be multiple of 16 bytes
	struct MixerConstant
	{
		MixerConstant(unsigned int _numNodes, float _ts)
			: ts(_ts), numNodes(_numNodes),pad0(0),pad1(0)
		{
		}

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
		BufferUAV_cs  mMixerResult;
		BufferCBV_cs  mMixerConstBuffer;
		MixerConstant *poMixerConstant;
	};
}

#endif

// --- End of File ---
