//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ROTATE_COMPONENT_H
#define ROTATE_COMPONENT_H

#include "GameComponentId.h"

namespace Azul
{
	// Spins an entity about Z. A game (app-authored) behavior component -- the
	// data-oriented replacement for the old Prefab_Rotate. RotateSystem reads
	// it + the entity's TransformComponent and rebuilds the world matrix.
	struct RotateComponent
	{
		float angle;   // current accumulated angle (radians)
		float speed;   // radians added per frame

		static const unsigned int kTypeId = COMPONENT_ROTATE;
	};
}

#endif

// --- End of File ---
