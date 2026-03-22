//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController_TwoAnim.h"
#include "Anim.h"

namespace Azul
{
	AnimController_TwoAnim::AnimController_TwoAnim(Anim *_pAnimA,
												   float ratioA,
												   Anim *_pAnimB,
												   float ratioB,
												   ComputeBlend_TwoAnim *_pBlend)
		:AnimController(),
		poAnimA{_pAnimA},
		poTimerControlA{nullptr},
		mRatioA{ratioA},
		poAnimB{_pAnimB},
		poTimerControlB{nullptr},
		mRatioB{ratioB},
		pBlend{_pBlend}
	{
		assert(this->poAnimA);
		assert(this->poAnimB);
		assert(this->pBlend);

		// TimerController
		AnimTime maxTime;
		AnimTime minTime;

		maxTime = poAnimA->FindMaxTime();
		minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControlA = new TimerController(minTime, maxTime);
		assert(this->poTimerControlA);

		maxTime = poAnimB->FindMaxTime();
		minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControlB = new TimerController(minTime, maxTime);
		assert(this->poTimerControlB);

	}

	AnimController_TwoAnim::~AnimController_TwoAnim()
	{
		delete poAnimA;
		delete poAnimB;
		delete poTimerControlA;
		delete poTimerControlB;
	}

	void AnimController_TwoAnim::Update(AnimTime tCurr)
	{
		this->poTimerControlA->Update(mRatioA * tCurr);
		this->pBlend->AnimateMixerA(this->poAnimA->GetClip(), this->poTimerControlA->GetCurrTime());

		this->poTimerControlB->Update(mRatioB * tCurr);
		this->pBlend->AnimateMixerB(this->poAnimB->GetClip(), this->poTimerControlB->GetCurrTime());

		this->pBlend->AnimateMixerC();
	}

	void AnimController_TwoAnim::SetBlendTs(float ts)
	{
		this->pBlend->SetBlendTs(ts);
	}


}

// --- End of File ---
