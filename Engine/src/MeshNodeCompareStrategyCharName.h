//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_NODE_Compare_Strategy_Char_Name_H
#define MESH_NODE_Compare_Strategy_Char_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class MeshNodeCompareStrategyCharName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API MeshNodeCompareStrategyCharName() = default;
		MANAGER_LIBRARY_API MeshNodeCompareStrategyCharName(const MeshNodeCompareStrategyCharName &) = default;
		MANAGER_LIBRARY_API MeshNodeCompareStrategyCharName &operator = (const MeshNodeCompareStrategyCharName &) = default;
		MANAGER_LIBRARY_API virtual ~MeshNodeCompareStrategyCharName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
