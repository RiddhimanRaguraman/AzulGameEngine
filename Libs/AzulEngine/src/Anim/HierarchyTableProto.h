#ifndef HIERARCHY_TABLE_PROTO_H
#define HIERARCHY_TABLE_PROTO_H
#include "EngineDLLInterface.h"

#include "HierarchyTable.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API HierarchyTableProto : public HierarchyTable
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

