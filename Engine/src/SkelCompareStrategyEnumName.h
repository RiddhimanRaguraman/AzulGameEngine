//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKEL_Compare_Strategy_Enum_Name_H
#define SKEL_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class SkelCompareStrategyEnumName : public CompareStrategyBase
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
