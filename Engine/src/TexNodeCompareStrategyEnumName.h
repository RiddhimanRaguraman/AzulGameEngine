//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Tex_NODE_Compare_Strategy_Enum_Name_H
#define Tex_NODE_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class TexNodeCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API TexNodeCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API TexNodeCompareStrategyEnumName(const TexNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API TexNodeCompareStrategyEnumName &operator = (const TexNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~TexNodeCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
