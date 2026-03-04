//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Camera_NODE_Compare_Strategy_Enum_Name_H
#define Camera_NODE_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class CameraNodeCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API CameraNodeCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API CameraNodeCompareStrategyEnumName(const CameraNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API CameraNodeCompareStrategyEnumName &operator = (const CameraNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~CameraNodeCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
