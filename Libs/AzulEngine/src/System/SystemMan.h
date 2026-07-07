//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ECS_SYSTEM_MAN_H
#define AZUL_ECS_SYSTEM_MAN_H

#include "EngineDLLInterface.h"
#include "AnimTime.h"

namespace Azul
{
	class System;
	class World;

	// Ordered list of engine systems. Run() drives them over the World each
	// frame (replacing the per-object virtual GameObject::Update walk). Owns the
	// systems it is given; Destroy() frees them (call before WorldMan::Destroy).
	class AZUL_ENGINE_LIBRARY_API SystemMan
	{
	public:
		SystemMan(const SystemMan &) = delete;
		SystemMan &operator=(const SystemMan &) = delete;

		static void Create();
		static void Destroy();

		static void Add(System *pSystem);              // takes ownership
		static void Run(World &world, AnimTime tCurr); // runs systems in order

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
