//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_RENDER_COMPONENT_H
#define AZUL_RENDER_COMPONENT_H

#include "ComponentId.h"

namespace Azul
{
	class GraphicsObject;

	// Per-entity render data. Phase 2 bridge: for now it just holds the
	// GraphicsObject* (the mesh+shader+material bundle) and the draw flag.
	// Phase 3/4 decomposes GraphicsObject into mesh/shader/tex handles here and
	// a RenderSystem, retiring the GraphicsObject_* hierarchy.
	// Ownership of the GraphicsObject stays with the GameObject (deleted in its
	// destructor); this component only references it.
	struct RenderComponent
	{
		GraphicsObject *pGraphicsObject;
		bool drawEnable;

		static const unsigned int kTypeId = COMPONENT_RENDER;
	};
}

#endif

// --- End of File ---
