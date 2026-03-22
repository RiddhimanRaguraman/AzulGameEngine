//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_ONE_ANIM_H
#define ANIM_CONTROLLER_ONE_ANIM_H

#include "AnimController.h"
#include "TimerController.h"
#include "Anim.h"
#include "ComputeBlend_OneAnim.h"

namespace Azul
{
	class AnimController_OneAnim : public AnimController
	{
	public:
		AnimController_OneAnim(Anim *pAnim, ComputeBlend_OneAnim *pBlend, float ratio);

		AnimController_OneAnim() = delete;
		AnimController_OneAnim(const AnimController_OneAnim &) = delete;
		AnimController_OneAnim &operator = (const AnimController_OneAnim &) = delete;
		virtual	~AnimController_OneAnim();

		virtual void Update(AnimTime tCurr) override;

	private:
		Anim* poAnimA;
		TimerController* poTimerControlA;
		float mRatioA;
		ComputeBlend_OneAnim *pBlend;
		

	};
}

#endif

// --- End of File ---
