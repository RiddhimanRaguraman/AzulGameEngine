//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HierarchyTable_Compare_Strategy_Enum_Name_H
#define HierarchyTable_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class HierarchyTableCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API HierarchyTableCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API HierarchyTableCompareStrategyEnumName(const HierarchyTableCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API HierarchyTableCompareStrategyEnumName &operator = (const HierarchyTableCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~HierarchyTableCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
