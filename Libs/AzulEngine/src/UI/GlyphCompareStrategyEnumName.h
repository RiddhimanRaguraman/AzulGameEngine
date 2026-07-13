#ifndef Glyph_Compare_Strategy_Enum_Name_H
#define Glyph_Compare_Strategy_Enum_Name_H

#include "Manager_DLLInterface.h"
#include "CompareStrategyBase.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API GlyphCompareStrategyEnumName : public CompareStrategyBase
	{
	public:

		MANAGER_LIBRARY_API GlyphCompareStrategyEnumName() = default;
		MANAGER_LIBRARY_API GlyphCompareStrategyEnumName(const GlyphCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API GlyphCompareStrategyEnumName &operator = (const GlyphCompareStrategyEnumName &) = default;
		MANAGER_LIBRARY_API virtual ~GlyphCompareStrategyEnumName() = default;

		MANAGER_LIBRARY_API virtual bool Compare(DLink *pRefNode, DLink *pTargetNode) override;

	};
}

#endif

// --- End of File ---
