//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "Anim.h"

namespace Azul
{
    AnimController::AnimController(Anim* pAnim, AnimTime delta)
    {
        assert(pAnim);
        // Animation object
        this->poAnim = pAnim;
        assert(this->poAnim);

        // TimerController
        AnimTime maxTime = poAnim->FindMaxTime();
        AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
        this->poTimerControl = new TimerController(delta, minTime, maxTime);
        assert(this->poTimerControl);

    }


	AnimController::~AnimController()
	{
		delete poAnim;
		delete poTimerControl;
	}

    void AnimController::Update()
    {
        this->poTimerControl->Update();

        // interpolate all bones
        this->poAnim->Animate(this->poTimerControl->GetCurrTime());

    }

    /*void AnimController::SetPivotScale(float sx, float sy, float sz)
    {
        this->poAnim->SetPivotScale(sx, sy, sz);
    }

    void AnimController::SetPivotUniformScale(float s)
    {
        this->poAnim->SetUniformPivotScale(s);
    }

    void AnimController::SetPivotTrans(float x, float y, float z)
    {
        this->poAnim->SetPivotTrans(x, y, z);
    }

	void AnimController::SetPivotRotX(float angle)
	{
		this->poAnim->SetPivotRotX(angle);
	}

    void AnimController::SetPivotRotY(float angle)
    {
        this->poAnim->SetPivotRotY(angle);
    }

    void AnimController::SetPivotRotZ(float angle)
    {
        this->poAnim->SetPivotRotZ(angle);
    }

    void AnimController::SetPivotTotalRot(const Rot3 mode, float x, float y, float z)
    {
        this->poAnim->SetPivotTotalRot(mode, x, y, z);
    }*/

    void AnimController::SetDelta(float scale)
    {
        AnimTime delta = scale * AnimTime(AnimTime::Duration::FILM_24_FRAME);
        this->poTimerControl->SetDeltaTime(delta);
    }

    void AnimController::SetClip(Clip::Name clipName)
    {
        if (this->poAnim)
        {
            this->poAnim->SetClip(clipName);
            if (this->poTimerControl)
            {
                AnimTime maxTime = this->poAnim->FindMaxTime();
                AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
                this->poTimerControl->SetCurrTime(minTime);
                this->poTimerControl->SetMaxTime(maxTime);
            }
        }
    }

    Clip *AnimController::GetClip()
    {
        return this->poAnim->GetClip();
    }
}

// --- End of File ---
