//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BONE_H
#define BONE_H
#include "EngineDLLInterface.h"

#include "MathEngine.h"

namespace Azul
{
	struct AZUL_ENGINE_LIBRARY_API Bone : public Align16
	{
		Bone() = default;
		Bone(const Bone&) = default;
		Bone& operator = (const Bone&) = default;
		~Bone() = default;

		Vec3  T;
		Quat  Q;
		Vec3  S;
	};
}

#endif

// --- End of File ---
