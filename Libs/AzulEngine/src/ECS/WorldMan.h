#ifndef AZUL_ECS_WORLD_MAN_H
#define AZUL_ECS_WORLD_MAN_H

#include "EngineDLLInterface.h"

namespace Azul
{
	class World;

	// Single engine-wide ECS World. Created lazily on first use and torn down
	// explicitly by Destroy() in the scene unload sequence -- BEFORE the framework's
	// leak check runs, so the World's pools/arrays are not reported as leaks.
	class AZUL_ENGINE_LIBRARY_API WorldMan
	{
	public:
		WorldMan() = delete;
		WorldMan(const WorldMan &) = delete;
		WorldMan &operator=(const WorldMan &) = delete;

		static World &GetWorld();
		static void Destroy();

	private:
		static World *posWorld;
	};
}

#endif

// --- End of File ---
