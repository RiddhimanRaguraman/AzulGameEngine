//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKEL_PROTO_H
#define SKEL_PROTO_H

#include "Skel.h"

namespace Azul
{
	class SkelProto
	{
	public:
		// Data
		SkelProto(const char *const pFileName, Skel::Name skelName);

		SkelProto() = delete;
		SkelProto(const SkelProto &) = delete;
		SkelProto &operator = (SkelProto &) = delete;
		virtual ~SkelProto();

	};
}

#endif

//--- End of File ---

