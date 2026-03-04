//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderObjectNodeCompareStrategyEnumName.h"
#include "ShaderObjectNode.h"

namespace Azul
{
	bool ShaderObjectNodeCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		ShaderObjectNode *pDataA = (ShaderObjectNode *)pRefNode;
		ShaderObjectNode *pDataB = (ShaderObjectNode *)pTargetNode;

		bool status = false;

		if(pDataA->GetShaderObject()->name == pDataB->GetShaderObject()->name)
		{
			status = true;
		}

		return status;
	}
}

// --- End of File ---
