//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef JointTable_Compare_Strategy_Enum_Name_H
#define JointTable_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class JointTableCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API JointTableCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API JointTableCompareStrategyEnumName(const JointTableCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API JointTableCompareStrategyEnumName &operator = (const JointTableCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~JointTableCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
