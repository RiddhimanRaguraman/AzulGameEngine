//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimManCompareStrategyEnumName.h"
#include "AnimMan.h"

namespace Azul
{
	bool AnimManCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);

		AnimMan::AnimNode *pDataA = (AnimMan::AnimNode *)pRefNode;
		AnimMan::AnimNode *pDataB = (AnimMan::AnimNode *)pTargetNode;

		bool status = false;

		if (pDataA->mName == pDataB->mName)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
