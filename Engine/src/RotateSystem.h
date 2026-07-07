//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ROTATE_SYSTEM_H
#define ROTATE_SYSTEM_H

#include "System.h"

namespace Azul
{
	// Game system: for every entity with a RotateComponent + TransformComponent,
	// advance its angle and rebuild world = Scale * baseRot * spinZ * Trans
	// (the behavior the dead Prefab_Rotate used to do). Runs AFTER
	// LocalToWorldSystem so its spin overwrites the plain S*R*T world.
	//
	// App-authored: it subclasses the engine's exported System base and is
	// registered with SystemMan from the scene. No AZUL_ENGINE_LIBRARY_API --
	// it lives in the app, not the DLL.
	class RotateSystem : public System
	{
	public:
		RotateSystem() = default;
		RotateSystem(const RotateSystem &) = delete;
		RotateSystem &operator=(const RotateSystem &) = delete;
		virtual ~RotateSystem();

		virtual void Update(World &world, AnimTime tCurr) override;
	};
}

#endif

// --- End of File ---
