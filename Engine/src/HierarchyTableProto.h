//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HIERARCHY_TABLE_PROTO_H
#define HIERARCHY_TABLE_PROTO_H

#include "HierarchyTable.h"

namespace Azul
{
	class HierarchyTableProto : public HierarchyTable
	{
	public:
		// Data
		HierarchyTableProto(const char *const pFileName);

		HierarchyTableProto() = delete;
		HierarchyTableProto(const HierarchyTableProto &) = delete;
		HierarchyTableProto &operator = (HierarchyTableProto &) = delete;
		virtual ~HierarchyTableProto();

	};
}

#endif

//--- End of File ---

