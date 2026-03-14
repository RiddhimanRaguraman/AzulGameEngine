//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HierarchyTableCompareStrategyEnumName.h"
#include "HierarchyTable.h"

namespace Azul
{
	bool HierarchyTableCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);

		HierarchyTable *pDataA = (HierarchyTable *)pRefNode;
		HierarchyTable *pDataB = (HierarchyTable *)pTargetNode;

		bool status = false;

		if(pDataA->mName == pDataB->mName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
