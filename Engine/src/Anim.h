//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_H
#define ANIM_H

#include "AnimTimer.h"
#include "MathEngine.h"
#include "Clip.h"
#include "Skeleton.h"
#include "TextureObject.h"
#include "JointTable.h"
#include "Mixer.h"

namespace Azul
{
    class Anim
    {
    public:
        Anim() = delete;
        Anim(const Anim &) = delete;
        Anim &operator = (const Anim &) = delete;
        ~Anim();

        Anim(Skeleton *ptSkeleton, Mixer *pMixer);

        void Animate(AnimTime tCurr);
        AnimTime FindMaxTime();

        void SetClip(Clip::Name clipName);
        Clip *GetClip();


    private:
        Skeleton *poSkeleton;
        Clip *pClip;
        Mixer *pMixer;
    };

}

#endif

// --- End of File ---
