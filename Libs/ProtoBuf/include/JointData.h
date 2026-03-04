//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef JOINT_DATA_H
#define JOINT_DATA_H

#include "ProtoBufEngine_DLLInterface.h"

#include "JointData.pb.h"

namespace Azul
{
	class JointData
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API JointData();
		PROTOBUF_ENGINE_LIBRARY_API JointData(const JointData&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API JointData& operator = (const JointData&);
		PROTOBUF_ENGINE_LIBRARY_API ~JointData();

		PROTOBUF_ENGINE_LIBRARY_API JointData(unsigned int _numBones, unsigned int* pData);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(JointData_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const JointData_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		unsigned int  numBones;
		unsigned int* poJointEntry;

	};

}

#endif

// --- End of File ---