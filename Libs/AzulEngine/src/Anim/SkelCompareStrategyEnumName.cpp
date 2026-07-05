//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SkelCompareStrategyEnumName.h"
#include "Skel.h"

namespace Azul
{
	bool SkelCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		Skel *pDataA = (Skel *)pRefNode;
		Skel *pDataB = (Skel *)pTargetNode;

		bool status = false;

		if(pDataA->mName == pDataB->mName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
