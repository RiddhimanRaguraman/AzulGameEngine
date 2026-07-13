#ifndef AZUL_ECS_SYSTEM_MAN_H
#define AZUL_ECS_SYSTEM_MAN_H

#include "EngineDLLInterface.h"
#include "AnimTime.h"

namespace Azul
{
	class System;
	class World;

	// The engine's per-scene ECS frame driver. Create() builds the ordered engine
	// system set; Run() drives the Update systems over the World each frame; Draw()
	// runs the render passes; Destroy() frees the systems (call before WorldMan::Destroy).
	class AZUL_ENGINE_LIBRARY_API SystemMan
	{
	public:
		SystemMan(const SystemMan &) = delete;
		SystemMan &operator=(const SystemMan &) = delete;

		// Allocate + populate the standard engine systems (LocalToWorld, Rotate,
		// Animation, Blend, Skinning).
		static void Create();
		static void Destroy();

		static void Add(System *pSystem);              // takes ownership
		static void Run(World &world, AnimTime tDelta); // runs Update systems in order
		static void Draw(World &world);                 // 3D pool pass, then 2D/UI pass

	private:
		SystemMan();
		~SystemMan() = default;
		static SystemMan *privInstance();

		static const unsigned int MAX_SYSTEMS = 32;
		System *poSystems[MAX_SYSTEMS];
		unsigned int mCount;

		static SystemMan *posInstance;
	};
}

#endif

// --- End of File ---
