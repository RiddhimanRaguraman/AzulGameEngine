//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectControlled.h"
#include "Anim.h"

namespace Azul
{
    class AnimController
    {
    public:
        AnimController(Anim* pAnim, AnimTime delta);
        AnimController(const AnimController &) = delete;
        AnimController &operator = (const AnimController &) = delete;
        virtual	~AnimController();

        void Update();

        //void SetPivotScale(float sx, float sy, float sz);
        //void SetPivotUniformScale(float s);
        //void SetPivotTrans(float x, float y, float z);
        //void SetPivotRotX(float angle);
        //void SetPivotRotY(float angle);
        //void SetPivotRotZ(float angle);
        //void SetPivotTotalRot(const Rot3 mode, float x, float y, float z);
        void SetDelta(float scale);
        void SetClip(Clip::Name clipName);
        Clip *GetClip();

	private:
		TimerController *poTimerControl;
		Anim *poAnim;

	};
}

#endif

// --- End of File ---
