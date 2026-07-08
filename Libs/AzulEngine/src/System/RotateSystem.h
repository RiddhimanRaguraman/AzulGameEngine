//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ROTATE_SYSTEM_H
#define ROTATE_SYSTEM_H

#include "EngineDLLInterface.h"
#include "System.h"

namespace Azul
{
	// Behavior system: for every entity with a RotateComponent + TransformComponent,
	// advance its angle and rebuild world = Scale * baseRot * spinZ * Trans
	// (the behavior the dead Prefab_Rotate used to do). Runs AFTER
	// LocalToWorldSystem so its spin overwrites the plain S*R*T world.
	//
	// Engine system: registered centrally in GameObjectMan::Create (no-op in
	// scenes without any RotateComponent).
	class AZUL_ENGINE_LIBRARY_API RotateSystem : public System
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
