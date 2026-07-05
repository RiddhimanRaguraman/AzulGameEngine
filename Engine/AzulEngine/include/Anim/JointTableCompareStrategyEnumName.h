//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef JointTable_Compare_Strategy_Enum_Name_H
#define JointTable_Compare_Strategy_Enum_Name_H
#include "EngineDLLInterface.h"

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API JointTableCompareStrategyEnumName : public CompareStrategyBase
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
