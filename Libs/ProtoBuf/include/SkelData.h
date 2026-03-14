//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef SKEL_DATA_H
#define SKEL_DATA_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "SkelData.pb.h"
#include "SkelEntry.h"

namespace Azul
{
	class SkelData
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API SkelData();
		PROTOBUF_ENGINE_LIBRARY_API SkelData(const SkelData&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API SkelData& operator = (const SkelData&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API ~SkelData();

		PROTOBUF_ENGINE_LIBRARY_API SkelData(unsigned int _numNodes);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(SkelData_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const SkelData_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		char           pVersion[protoVersion::VERSION_NUM_BYTES];
		unsigned int   numNodes;
		SkelEntry* poSkelEntry;
	};

}

#endif

// --- End of File ---
