//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Image_Compare_Strategy_Enum_Name_H
#define Image_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class ImageCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API ImageCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API ImageCompareStrategyEnumName(const ImageCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API ImageCompareStrategyEnumName &operator = (const ImageCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~ImageCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
