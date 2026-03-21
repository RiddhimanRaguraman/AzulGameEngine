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
#include "HierarchyTable.h"
#include "WorldCompute.h"

namespace Azul
{
    class Anim
    {
    public:
        Anim() = delete;
        Anim(const Anim &) = delete;
        Anim &operator = (const Anim &) = delete;
        ~Anim();

        Anim(HierarchyTable::Name hierarchyName, Clip::Name clipName);

        void Animate(AnimTime tCurr);
        AnimTime FindMaxTime();

        void SetClip(Clip::Name clipName);
        Mixer* GetMixer();
        WorldCompute* GetWorldCompute();
        Clip *privGetClip();


    private:
        Skeleton *poSkeleton;
        Mixer* poMixer;
        WorldCompute* poWorldCompute;
    };

}

#endif

// --- End of File ---
