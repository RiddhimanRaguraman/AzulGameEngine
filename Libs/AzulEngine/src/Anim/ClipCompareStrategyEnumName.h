#ifndef Clip_Compare_Strategy_Enum_Name_H
#define Clip_Compare_Strategy_Enum_Name_H
#include "EngineDLLInterface.h"

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API ClipCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API ClipCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API ClipCompareStrategyEnumName(const ClipCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API ClipCompareStrategyEnumName &operator = (const ClipCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~ClipCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
