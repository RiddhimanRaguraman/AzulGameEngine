//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "JointTableCompareStrategyEnumName.h"
#include "JointTable.h"

namespace Azul
{
	bool JointTableCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		JointTable *pDataA = (JointTable *)pRefNode;
		JointTable *pDataB = (JointTable *)pTargetNode;

		bool status = false;

	if(pDataA->mName == pDataB->mName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
