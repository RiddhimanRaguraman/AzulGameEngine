//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_TWO_ANIM_H
#define ANIM_CONTROLLER_TWO_ANIM_H

#include "AnimController.h"
#include "TimerController.h"
#include "Anim.h"
#include "ComputeBlend_TwoAnim.h"

namespace Azul
{
	class AnimController_TwoAnim : public AnimController
	{
	public:
		AnimController_TwoAnim(Anim *pAnimA, 
							   float ratioA, 
							   Anim *pAnimB, 
							   float ratioB, 
							   ComputeBlend_TwoAnim *pBlend);

		AnimController_TwoAnim() = delete;
		AnimController_TwoAnim(const AnimController_TwoAnim &) = delete;
		AnimController_TwoAnim &operator = (const AnimController_TwoAnim &) = delete;
		virtual	~AnimController_TwoAnim();

		virtual void Update(AnimTime tCurr) override;
		void SetBlendTs(float ts);

	private:

		Anim *poAnimA;
		TimerController *poTimerControlA;
		float mRatioA;

		Anim *poAnimB;		
		TimerController *poTimerControlB;
		float mRatioB;

		ComputeBlend_TwoAnim *pBlend;


	};
}

#endif

// --- End of File ---
