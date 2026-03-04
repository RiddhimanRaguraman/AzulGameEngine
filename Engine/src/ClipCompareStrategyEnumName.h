//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Clip_Compare_Strategy_Enum_Name_H
#define Clip_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class ClipCompareStrategyEnumName : public CompareStrategyBase
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
