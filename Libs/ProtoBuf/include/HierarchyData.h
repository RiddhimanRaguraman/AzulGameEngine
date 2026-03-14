//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef HIERACRCHY_DATA_H
#define HIERACRCHY_DATA_H

#include "ProtoBufEngine_DLLInterface.h" 

#include "HierarchyData.pb.h"
#include "ProtoVersion.h"

namespace Azul
{
	class HierarchyData
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API HierarchyData();
		PROTOBUF_ENGINE_LIBRARY_API HierarchyData(const HierarchyData &) = delete;
		PROTOBUF_ENGINE_LIBRARY_API HierarchyData &operator = (const HierarchyData &);
		PROTOBUF_ENGINE_LIBRARY_API ~HierarchyData();

		PROTOBUF_ENGINE_LIBRARY_API HierarchyData(unsigned int _numNodes, unsigned int _maxDepth);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(HierarchyData_proto &out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const HierarchyData_proto &in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char *const pName) const;

	public:
		char  pVersion[protoVersion::VERSION_NUM_BYTES];
		unsigned int numNodes;
		unsigned int maxDepth;
		unsigned int *poData;
	};

}

#endif

// --- End of File ---
