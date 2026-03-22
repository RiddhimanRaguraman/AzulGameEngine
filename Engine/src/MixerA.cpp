//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MixerA.h"
#include "BufferSRV_cs.h"
#include "Clip.h"

namespace Azul
{
	MixerA::MixerA(Clip* pClip)
		:Mixer(pClip),
		pKeyAa{ nullptr },
		pKeyAb{ nullptr }
	{
		assert(pClip);
	}

	MixerA::~MixerA()
	{
		// nothing since the buffers are objects
	}

}

// --- End of File ---