//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TimerController.h"

namespace Azul
{

	TimerController::TimerController(AnimTime  Min, AnimTime Max)
		: tCurr(AnimTime::Duration::ZERO),
		tMax(Max),
		tMin(Min)
	{
	}

	void TimerController::Update(AnimTime tDelta)
	{
		tCurr += tDelta;

		// protection for time values for looping
		if(tCurr < tMin)
		{
			tCurr = tMax;
		}
		else if(tCurr > tMax)
		{
			tCurr = tMin;
		}
		else
		{
			// do nothing
		}
	}

	void TimerController::SetCurrTime(const AnimTime time)
	{
		this->tCurr = time;
	}

	AnimTime TimerController::GetCurrTime() const
	{
		return this->tCurr;
	}

	void TimerController::SetMaxTime(const AnimTime time)
	{
		this->tMax = time;
	}

	AnimTime TimerController::GetMaxTime() const
	{
		return this->tMax;
	}

	void TimerController::SetMinTime(const AnimTime time)
	{
		this->tMin = time;
	}

	AnimTime TimerController::GetMinTime() const
	{
		return this->tMin;
	}

}

// --- End of File ---
