//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_H
#define ANIM_H

#include "AnimTimer.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{
    class Anim
    {
    public:
        Anim() = delete;
        Anim(const Anim &) = delete;
        Anim &operator = (const Anim &) = delete;
        ~Anim();

        Anim(Clip::Name clipName);

        AnimTime FindMaxTime();

        void SetClip(Clip::Name clipName);
        Clip *GetClip();


    private:
        Skeleton *poSkeleton;
        Clip* pClip;
        Clip::Name mClipName;
    };

}

#endif

// --- End of File ---
