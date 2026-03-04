//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef FONT_ENTRY_H
#define FONT_ENTRY_H

#include "ProtoBufEngine_DLLInterface.h"

#include "FontEntry.pb.h"

namespace Azul
{
	class FontEntry
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API FontEntry();
		PROTOBUF_ENGINE_LIBRARY_API FontEntry(const FontEntry&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API FontEntry& operator = (const FontEntry&) = default;
		PROTOBUF_ENGINE_LIBRARY_API ~FontEntry();

		PROTOBUF_ENGINE_LIBRARY_API FontEntry(unsigned int key, float x, float y, float width, float height);
		PROTOBUF_ENGINE_LIBRARY_API void set(unsigned int key, float x, float y, float width, float height);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(FontEntry_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const FontEntry_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		unsigned int key;
		float x;
		float y;
		float width;
		float height;
	};
}


#endif

// --- End of File ---
