#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "MathEngine.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	struct AZUL_ENGINE_LIBRARY_API Sphere
	{
		Vec3 cntr;
		float rad;
		float pad[3];  // love and hate this new compiler
	};

	void RitterSphere(Sphere &s, Vec3 *pt, int numPts);
}

#endif

// --- End of File ---
