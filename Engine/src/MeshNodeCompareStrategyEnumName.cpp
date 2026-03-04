//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshNodeCompareStrategyEnumName.h"
#include "MeshNode.h"

namespace Azul
{

	bool MeshNodeCompareStrategyEnumName::Compare(DLink *pRefNode,DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		MeshNode *pDataA = (MeshNode *)pRefNode;
		MeshNode *pDataB = (MeshNode *)pTargetNode;

		bool status = false;

		if(pDataA->GetMesh()->name == pDataB->GetMesh()->name)
		{
			status = true;
		}

		return status;
	}

}

// --- End of File ---
