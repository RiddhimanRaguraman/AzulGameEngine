//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MixerC.h"
#include "BufferUAV_cs.h"
#include "Clip.h"

namespace Azul
{
	MixerC::MixerC(Clip *pClip)
		:Mixer(pClip),
		pKeyCa{nullptr},
		pKeyCb{nullptr}
	{
		assert(pClip);
		// clip was only used to get number of Nodes
	}

	MixerC::~MixerC()
	{
		// nothing since the buffers are objects
	}

}

// --- End of File ---
