
#ifndef AZUL_LIGHT_COMPONENT_H
#define AZUL_LIGHT_COMPONENT_H

#include "MathEngine.h"
#include "ComponentId.h"

namespace Azul
{
	// A light source (color + world position). Lifted out of the hard-coded
	// poLightColor/poLightPos that used to live inside the (now-deleted)
	// GraphicsObject_LightTexture.
	// Keyed by a light entity the material owns; a later phase can promote this
	// to a first-class scene light consumed by a lighting/render system.
	struct LightComponent
	{
		Vec3 color;
		Vec3 pos;

		static const unsigned int kTypeId = COMPONENT_LIGHT;
	};
}

#endif

// --- End of File ---
