//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WORLD_CONSTANT_H
#define WORLD_CONSTANT_H

namespace Azul
{
	// structure for Constant Compute buffer needs to be multiple of 16 bytes
	struct WorldConstant
	{
		unsigned int hierarchyDepth;
		unsigned int numJoints;
		int pad0;
		int pad1;
	};
}

#endif

// --- End of File ---
