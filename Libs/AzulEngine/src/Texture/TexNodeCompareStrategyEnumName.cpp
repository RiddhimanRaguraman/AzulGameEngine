//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TexNodeCompareStrategyEnumName.h"
#include "TexNode.h"

namespace Azul
{
	bool TexNodeCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		TexNode *pDataA = (TexNode *)pRefNode;
		TexNode *pDataB = (TexNode *)pTargetNode;

		bool status = false;

		if(pDataA->GetTextureObject()->mName == pDataB->GetTextureObject()->mName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
