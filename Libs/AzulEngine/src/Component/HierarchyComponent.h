//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_HIERARCHY_COMPONENT_H
#define AZUL_HIERARCHY_COMPONENT_H

#include "Entity.h"
#include "ComponentId.h"

namespace Azul
{
	// Per-entity scene-graph link: the parent entity (EntityNull for a root).
	//
	// Phase 2 bridge: this MIRRORS the PCSTree parent that GameObjectMan still
	// owns and traverses. Phase 5 makes it authoritative and adds a
	// TransformPropagationSystem that composes local->world parent-before-child.
	struct HierarchyComponent
	{
		Entity parent;

		static const unsigned int kTypeId = COMPONENT_HIERARCHY;
	};
}

#endif

// --- End of File ---
