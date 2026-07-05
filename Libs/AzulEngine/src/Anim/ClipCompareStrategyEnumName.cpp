//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ClipCompareStrategyEnumName.h"
#include "Clip.h"

namespace Azul
{
	bool ClipCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		Clip *pDataA = (Clip *)pRefNode;
		Clip *pDataB = (Clip *)pTargetNode;

		bool status = false;

	if(pDataA->mClipName == pDataB->mClipName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
