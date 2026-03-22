//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController_OneAnim.h"
#include "Anim.h"

namespace Azul
{
	Anim* poAnimA;
	TimerController* poTimerControlA;
	float mRatioA;

	ComputeBlend_OneAnim* pBlend;

	AnimController_OneAnim::AnimController_OneAnim(Anim *_pAnim, ComputeBlend_OneAnim *_pBlend, float ratio)
		:AnimController(),
		poAnimA{ _pAnim },
		poTimerControlA{ nullptr },
		mRatioA{ ratio },
		pBlend{ _pBlend }
	{
		assert(this->poAnimA);
		assert(this->pBlend);

		// TimerController
		AnimTime maxTime = poAnimA->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControlA = new TimerController(minTime, maxTime);
		assert(this->poTimerControlA);
	}

	AnimController_OneAnim::~AnimController_OneAnim()
	{
		delete this->poAnimA;
		this->poAnimA = nullptr;
		delete this->poTimerControlA;
		this->poTimerControlA = nullptr;
	}

	void AnimController_OneAnim::Update(AnimTime tCurr)
	{
		this->poTimerControlA->Update(mRatioA * tCurr);

		// interpolate all bones
		this->pBlend->AnimateMixerA(this->poAnimA->GetClip(), this->poTimerControlA->GetCurrTime());

	}
}

// --- End of File ---
