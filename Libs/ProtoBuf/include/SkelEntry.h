//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef SKEL_ENTRY_H
#define SKEL_ENTRY_H

#include "ProtoBufEngine_DLLInterface.h"

#include "SkelEntry.pb.h"

namespace Azul
{

	class SkelEntry
	{
	public:
		static const unsigned int SKELETON_NAME_SIZE = 64;

	public:
		PROTOBUF_ENGINE_LIBRARY_API SkelEntry();
		PROTOBUF_ENGINE_LIBRARY_API SkelEntry(const SkelEntry&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API SkelEntry& operator = (const SkelEntry&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API ~SkelEntry();

		PROTOBUF_ENGINE_LIBRARY_API void set(int _nodeIndex,
			int   _parentIndex,
			char* pNodeName);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(SkelEntry_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const SkelEntry_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		int   nodeIndex;
		int   parentIndex;
		char  pNodeName[SKELETON_NAME_SIZE];

	};

}

#endif

// --- End of File ---
