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

namespace Azul
{
    class Anim
    {
    public:
        Anim() = delete;
        Anim(const Anim &) = delete;
        Anim &operator = (const Anim &) = delete;
        ~Anim();

        Anim(Skeleton *ptSkeleton);

        //Anim(Clip::Name clipName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable);

        void Animate(AnimTime tCurr);
        AnimTime FindMaxTime();

        void SetClip(Clip::Name clipName);
        Clip *GetClip();

        void SetPivotScale(float sx, float sy, float sz);
        void SetUniformPivotScale(float s);
        void SetPivotTrans(float x, float y, float z);
        void SetPivotRotX(float angle);
        void SetPivotRotY(float angle);
        void SetPivotRotZ(float angle);
        void SetPivotTotalRot(const Rot3 mode, float x, float y, float z);

    private:

        Skeleton *poSkeleton;
    };

}

#endif

// --- End of File ---
