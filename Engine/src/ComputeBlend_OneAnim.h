//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef COMPUTE_BLEND_ONE_ANIM_H
#define COMPUTE_BLEND_ONE_ANIM_H

#include "ComputeBlend.h"
#include "MixerA.h"
#include "WorldComputeA.h"

namespace Azul
{
	class Anim;

	class ComputeBlend_OneAnim : public ComputeBlend
	{
	public:
		ComputeBlend_OneAnim(Anim *ptAnim);

		ComputeBlend_OneAnim() = delete;
		ComputeBlend_OneAnim(const ComputeBlend_OneAnim &) = delete;
		ComputeBlend_OneAnim &operator = (const ComputeBlend_OneAnim &) = delete;
		virtual ~ComputeBlend_OneAnim();

		virtual void Execute() override;
		virtual void AnimateMixerA(Clip *pClip, AnimTime time) ;
		virtual void BindWorldBoneArray() override;


	private:
		void privMixerExecute();
		void privWorldComputeExecute();

	public:
		MixerA*		  poMixerA;
		WorldComputeA* poWorldComputeA;
	};
}

#endif

// --- End of File ---
