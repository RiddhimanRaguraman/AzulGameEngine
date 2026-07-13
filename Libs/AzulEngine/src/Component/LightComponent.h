
#ifndef AZUL_LIGHT_COMPONENT_H
#define AZUL_LIGHT_COMPONENT_H

#include "MathEngine.h"
#include "ComponentId.h"

namespace Azul
{
	// A light source (color + world position).
	struct LightComponent
	{
		Vec3 color;
		Vec3 pos;

		static const unsigned int kTypeId = COMPONENT_LIGHT;
	};
}

#endif

// --- End of File ---
