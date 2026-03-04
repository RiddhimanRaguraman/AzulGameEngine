//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ImageCompareStrategyEnumName.h"
#include "Image.h"

namespace Azul
{
	bool ImageCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		Image *pDataA = (Image *)pRefNode;
		Image *pDataB = (Image *)pTargetNode;

		bool status = false;

	if(pDataA->imageName == pDataB->imageName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
