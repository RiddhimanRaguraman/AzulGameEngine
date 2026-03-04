//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Shader_OBject_NODE_Compare_Strategy_Enum_Name_H
#define Shader_OBject_NODE_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

namespace Azul
{
	class ShaderObjectNodeCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API ShaderObjectNodeCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API ShaderObjectNodeCompareStrategyEnumName(const ShaderObjectNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API ShaderObjectNodeCompareStrategyEnumName &operator = (const ShaderObjectNodeCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~ShaderObjectNodeCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
