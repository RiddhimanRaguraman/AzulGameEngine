//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_NODE_Compare_Strategy_Enum_Name_H
#define MESH_NODE_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class MeshNodeCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API MeshNodeCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API MeshNodeCompareStrategyEnumName(const MeshNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API MeshNodeCompareStrategyEnumName &operator = (const MeshNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~MeshNodeCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
