//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef COMPUTE_BLEND_TWO_ANIM_H
#define COMPUTE_BLEND_TWO_ANIM_H

#include "ComputeBlend.h"
#include "MixerA.h"
#include "MixerB.h"
#include "MixerC.h"
#include "WorldComputeC.h"

namespace Azul
{
	class Anim;

	class ComputeBlend_TwoAnim : public ComputeBlend
	{
	public:
		ComputeBlend_TwoAnim(Anim *pAnim_A, Anim *pAnim_B);

		ComputeBlend_TwoAnim() = delete;
		ComputeBlend_TwoAnim(const ComputeBlend_TwoAnim &) = delete;
		ComputeBlend_TwoAnim &operator = (const ComputeBlend_TwoAnim &) = delete;
		virtual ~ComputeBlend_TwoAnim();

		virtual void Execute() override;

		virtual void AnimateMixerA(Clip *pClip, AnimTime time);
		virtual void AnimateMixerB(Clip *pClip, AnimTime time);
		virtual void AnimateMixerC();

		void SetBlendTs(float ts);
		float GetBlendTs() const;

		virtual void BindWorldBoneArray() override;

	private:
		void privMixerExecute();
		void privWorldComputeExecute();

	public:
		float mBlendTs;
		MixerA *poMixerA;
		MixerB *poMixerB;
		MixerC *poMixerC;
		WorldComputeC* poWorldComputeC;
	};
}

#endif

// --- End of File ---
