//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshNodeCompareStrategyCharName.h"
#include "MeshNode.h"

namespace Azul
{

	bool MeshNodeCompareStrategyCharName::Compare(DLink *pRefNode,DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		MeshNode *pDataA = (MeshNode *)pRefNode;
		MeshNode *pDataB = (MeshNode *)pTargetNode;

		bool status = false;

		if( strcmp(pDataA->GetMesh()->pModelName, pDataB->GetMesh()->pModelName) == 0 )
		{
			status = true;
		}

		return status;
	}

}

// --- End of File ---
