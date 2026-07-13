#ifndef AZUL_BLEND_SYSTEM_H
#define AZUL_BLEND_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Advances every two-clip blended animation: for each AnimBlendComponent it
	// drives the controller (both timers advance, both clips sample, MixerC
	// blends into the mixer key buffers), in one flat pass over the pool.
	// Replaces AnimMan::Update's OOP driving of the two-anim controller.
	//
	// Time: driven with the frame DELTA (see SystemMan::Run).
	// Ordering: like AnimationSystem, must run BEFORE SkinningSystem (sample the
	// mixer, then dispatch the compute skinning that consumes it).
	class AZUL_ENGINE_LIBRARY_API BlendSystem : public System
	{
	public:
		BlendSystem() = default;
		BlendSystem(const BlendSystem &) = delete;
		BlendSystem &operator=(const BlendSystem &) = delete;
		virtual ~BlendSystem();

		virtual void Update(World &world, AnimTime tDelta) override;
	};
}

#endif

// --- End of File ---
