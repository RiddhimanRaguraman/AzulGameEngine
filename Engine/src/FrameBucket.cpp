//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "FrameBucket.h"
  
namespace Azul
{
	FrameBucket::FrameBucket()
		: nextBucket{nullptr},
		prevBucket{nullptr},
		KeyTime{AnimTime(AnimTime::Duration::ZERO)},
		poBone{nullptr},
		pad{0}
	{

	}

	FrameBucket::~FrameBucket()
	{
		delete[] poBone;
	}
}

// --- End of File ---
