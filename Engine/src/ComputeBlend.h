//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef COMPUTE_BLEND_H
#define COMPUTE_BLEND_H

#include "AnimTime.h"
#include "Clip.h"

namespace Azul
{
	class ComputeBlend
	{
	public:
		ComputeBlend() = default;
		ComputeBlend(const ComputeBlend&) = delete;
		ComputeBlend& operator = (const ComputeBlend&) = delete;
		virtual ~ComputeBlend() = default;

		virtual void Execute() = 0;
		virtual void BindWorldBoneArray() = 0;

	public:

	};
}

#endif

// --- End of File ---
