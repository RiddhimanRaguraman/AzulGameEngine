//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MAN_COMPARE_STRATEGY_ENUM_NAME_H
#define ANIM_MAN_COMPARE_STRATEGY_ENUM_NAME_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class AnimManCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API AnimManCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API AnimManCompareStrategyEnumName(const AnimManCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API AnimManCompareStrategyEnumName &operator = (const AnimManCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~AnimManCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
