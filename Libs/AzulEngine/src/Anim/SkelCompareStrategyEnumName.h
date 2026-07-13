#ifndef SKEL_Compare_Strategy_Enum_Name_H
#define SKEL_Compare_Strategy_Enum_Name_H
#include "EngineDLLInterface.h"

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API SkelCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API SkelCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API SkelCompareStrategyEnumName(const SkelCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API SkelCompareStrategyEnumName &operator = (const SkelCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~SkelCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
