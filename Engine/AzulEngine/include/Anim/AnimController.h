//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H
#include "EngineDLLInterface.h"

#include "AnimTime.h"

namespace Azul
{
    class AZUL_ENGINE_LIBRARY_API AnimController
    {
    public:

        AnimController() = default;
        AnimController(const AnimController &) = delete;
        AnimController &operator = (const AnimController &) = delete;
        virtual	~AnimController() ;

        virtual void Update(AnimTime tCurr) = 0;
       /* void SetClip(Clip::Name clipName);
        Clip *GetClip();*/

	};
}

#endif

// --- End of File ---
