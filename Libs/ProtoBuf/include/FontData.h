//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef FONT_DATA_H
#define FONT_DATA_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "FontEntry.h"
#include "FontData.pb.h"

namespace Azul
{
	class FontData
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API FontData();
		PROTOBUF_ENGINE_LIBRARY_API FontData(const FontData&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API FontData& operator = (const FontData&);
		PROTOBUF_ENGINE_LIBRARY_API ~FontData();

		PROTOBUF_ENGINE_LIBRARY_API FontData(unsigned int _numKeys);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(FontData_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const FontData_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		char          pVersion[protoVersion::VERSION_NUM_BYTES];
		unsigned int numKeys;
		FontEntry* poFontEntry;
	};
}


#endif

// --- End of File ---
