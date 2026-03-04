//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_HELPER_H
#define GLTF_HELPER_H

namespace Azul
{
	class StringThis
	{
	public:
		static const unsigned int BUFFER_SIZE = 64;
	public:
		StringThis(int x);

		operator char *();

		// data:
		char buffer[BUFFER_SIZE];
	};
}

#define StringMe(x)  ((char *)StringThis(x)) 

#endif

// --- End of File ---
