//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_A_H
#define MIXER_A_H

#include "Mixer.h"


namespace Azul
{
	class Clip;
	class BufferSRV_cs;

	class MixerA : public Mixer
	{
	public:
		MixerA(Clip* pClip);

		MixerA() = delete;
		MixerA(const MixerA&) = delete;
		MixerA& operator = (const MixerA&) = delete;
		virtual ~MixerA();

	public:

		BufferSRV_cs* pKeyAa;
		BufferSRV_cs* pKeyAb;

	};
}

#endif

// --- End of File ---