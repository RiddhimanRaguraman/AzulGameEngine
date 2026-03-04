//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef JOINT_TABLE_PROTO_H
#define JOINT_TABLE_PROTO_H

#include "JointTable.h"

namespace Azul
{
	class JointTableProto : public JointTable
	{
	public:
		// Data
		JointTableProto(const char *const pFileName);

		JointTableProto() = delete;
		JointTableProto(const JointTableProto &) = delete;
		JointTableProto &operator = (JointTableProto &) = delete;
		virtual ~JointTableProto();

	};
}

#endif

//--- End of File ---

