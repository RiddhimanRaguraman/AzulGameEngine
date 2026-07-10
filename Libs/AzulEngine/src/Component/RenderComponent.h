//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_RENDER_COMPONENT_H
#define AZUL_RENDER_COMPONENT_H

#include "ComponentId.h"
#include "MaterialKind.h"

namespace Azul
{
	class GraphicsObject;

	// Per-entity render data. Phase 4 (in progress): `kind` is the data tag the
	// RenderSystem/SpriteRenderSystem branch on; `pGraphicsObject` is still the
	// bridge that actually issues the draw until P4.2 decomposes it into
	// mesh/shader/tex handles and the GraphicsObject_* hierarchy is retired.
	// Ownership of the GraphicsObject stays with the GameObject (deleted in its
	// destructor); this component only references it.
	struct RenderComponent
	{
		GraphicsObject *pGraphicsObject;
		MaterialKind kind;
		bool drawEnable;

		static const unsigned int kTypeId = COMPONENT_RENDER;
	};
}

#endif

// --- End of File ---
