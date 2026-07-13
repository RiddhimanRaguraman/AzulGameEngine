#ifndef MESH_NODE_Compare_Strategy_Char_Name_H
#define MESH_NODE_Compare_Strategy_Char_Name_H
#include "EngineDLLInterface.h"

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API MeshNodeCompareStrategyCharName : public CompareStrategyBase
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
