#ifndef AZUL_ANIMATION_SYSTEM_H
#define AZUL_ANIMATION_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Advances every single-clip animation: for each AnimClipComponent it drives
	// the controller (timer advance + clip sample into the mixer key buffers), in
	// one flat pass over the pool. Replaces AnimMan::Update's per-node list walk
	// for the one-anim path.
	//
	// Time: driven with the frame DELTA (see SystemMan::Run),
	// because the controllers' TimerController accumulates (tCurr += tDelta).
	//
	// Ordering: must run BEFORE SkinningSystem (sample the mixer, then dispatch the
	// compute skinning that consumes it). Registered accordingly in SystemMan::Create.
	class AZUL_ENGINE_LIBRARY_API AnimationSystem : public System
	{
	public:
		AnimationSystem() = default;
		AnimationSystem(const AnimationSystem &) = delete;
		AnimationSystem &operator=(const AnimationSystem &) = delete;
		virtual ~AnimationSystem();

		virtual void Update(World &world, AnimTime tDelta) override;
	};
}

#endif

// --- End of File ---
