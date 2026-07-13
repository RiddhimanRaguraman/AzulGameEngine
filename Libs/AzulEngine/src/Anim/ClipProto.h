#ifndef CLIP_PROTO_H
#define CLIP_PROTO_H
#include "EngineDLLInterface.h"

#include "Clip.h"
#include "HierarchyTable.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API ClipProto
	{
	public:
		// Data
		ClipProto(const char *const pFileName, Clip::Name clipName, Skel::Name skelName, HierarchyTable::Name hierarchyName);

		ClipProto() = delete;
		ClipProto(const ClipProto &) = delete;
		ClipProto &operator = (ClipProto &) = delete;
		virtual ~ClipProto();

	};
}

#endif

//--- End of File ---

