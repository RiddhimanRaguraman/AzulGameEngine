//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "FrameBucket.h"
#include "BufferSRV_cs.h"

namespace Azul
{
	FrameBucket::FrameBucket()
		: nextBucket{ nullptr },
		prevBucket{ nullptr },
		KeyTime{ AnimTime(AnimTime::Duration::ZERO) },
		poBoneSRV{ nullptr }
	{
		// poBoneSRV is set in Clip loading
	}

	FrameBucket::~FrameBucket()
	{
		delete this->poBoneSRV;
		this->poBoneSRV = nullptr;
	}
}

// --- End of File ---
