//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FRAME_BUCKET_H
#define FRAME_BUCKET_H

#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
	struct FrameBucket
	{
		FrameBucket();
		FrameBucket(const FrameBucket &) = delete;
		FrameBucket &operator=(const FrameBucket &) = delete;
		~FrameBucket();

		FrameBucket  *nextBucket;
		FrameBucket  *prevBucket;
		AnimTime      KeyTime;
		Bone         *poBone;
		char          pad[4];
	};
}

#endif

// --- End of File ---
