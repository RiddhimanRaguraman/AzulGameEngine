//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MixerB.h"
#include "BufferSRV_cs.h"
#include "Clip.h"

namespace Azul
{
	MixerB::MixerB(Clip *pClip)
		:Mixer(pClip),
		pKeyBa{nullptr},
		pKeyBb{nullptr}
	{
		assert(pClip);
	}

	MixerB::~MixerB()
	{
		// nothing since the buffers are objects
	}

}

// --- End of File ---
