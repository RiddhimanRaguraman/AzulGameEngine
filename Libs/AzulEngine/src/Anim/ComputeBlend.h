#ifndef COMPUTE_BLEND_H
#define COMPUTE_BLEND_H
#include "EngineDLLInterface.h"

#include "AnimTime.h"
#include "Clip.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API ComputeBlend
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
