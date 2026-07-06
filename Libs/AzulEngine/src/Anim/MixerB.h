//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MIXER_B_H
#define MIXER_B_H
#include "EngineDLLInterface.h"

#include "Mixer.h"

namespace Azul
{
	class Clip;
	class BufferSRV_cs;

	class AZUL_ENGINE_LIBRARY_API MixerB : public Mixer
	{
	public:
		MixerB(Clip *pClip);

		MixerB() = delete;
		MixerB(const MixerB &) = delete;
		MixerB &operator = (const MixerB &) = delete;
		virtual ~MixerB();

	public:
		BufferSRV_cs *pKeyBa;
		BufferSRV_cs *pKeyBb;
	};
}

#endif

// --- End of File ---
