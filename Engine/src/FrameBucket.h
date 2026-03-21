//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FRAME_BUCKET_H
#define FRAME_BUCKET_H

#include "AnimTime.h"

namespace Azul
{
	class BufferSRV_cs;

	class FrameBucket
	{
	public:
		FrameBucket();
		FrameBucket(const FrameBucket &) = delete;
		FrameBucket &operator=(const FrameBucket &) = delete;
		~FrameBucket();

		FrameBucket  *nextBucket;
		FrameBucket  *prevBucket;
		AnimTime      KeyTime;
		BufferSRV_cs *poBoneSRV;
	};
}

#endif

// --- End of File ---
