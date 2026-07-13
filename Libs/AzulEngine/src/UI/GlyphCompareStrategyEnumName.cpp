#include "GlyphCompareStrategyEnumName.h"
#include "Glyph.h"

namespace Azul
{
	bool GlyphCompareStrategyEnumName::Compare(DLink *pRefNode, DLink *pTargetNode)
	{
		assert(pRefNode);
		assert(pTargetNode);
		
		Glyph *pDataA = (Glyph *)pRefNode;
		Glyph *pDataB = (Glyph *)pTargetNode;

		bool status = false;

		if(pDataA->glyphName == pDataB->glyphName)
		{
			if(pDataA->key == pDataB->key)
			{
				status = true;
			}
		}

		return status;
	}
}

// --- End of File ---
