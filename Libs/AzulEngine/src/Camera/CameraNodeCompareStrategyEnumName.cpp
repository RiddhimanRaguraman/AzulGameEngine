//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraNodeCompareStrategyEnumName.h"
#include "CameraNode.h"

namespace Azul
{
	bool CameraNodeCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		CameraNode *pDataA = (CameraNode *)pRefNode;
		CameraNode *pDataB = (CameraNode *)pTargetNode;

		bool status = false;

		if(pDataA->GetCamera()->name == pDataB->GetCamera()->name)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---

