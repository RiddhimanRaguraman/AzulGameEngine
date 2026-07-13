#ifndef AZUL_ECS_SMOKE_TEST_H
#define AZUL_ECS_SMOKE_TEST_H

#include "EngineDLLInterface.h"

namespace Azul
{
	// Self-check for the ECS core. Call once at startup; confirm the asserts pass
	// and the trace prints "ECS smoke test PASSED".
	AZUL_ENGINE_LIBRARY_API void ECS_SmokeTest();
}

#endif

// --- End of File ---
