#ifndef ANIM_H
#define ANIM_H
#include "EngineDLLInterface.h"

#include "AnimTimer.h"
#include "Clip.h"
#include "Skeleton.h"

namespace Azul
{
    class AZUL_ENGINE_LIBRARY_API Anim
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
