#ifndef FRAMEBUCKET_H
#define FRAMEBUCKET_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "AnimFrameBucket.pb.h"
#include "AnimBone.h"

namespace Azul 
{
	class AnimFrameBucket
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API AnimFrameBucket();
		PROTOBUF_ENGINE_LIBRARY_API AnimFrameBucket(const AnimFrameBucket&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API AnimFrameBucket& operator = (const AnimFrameBucket&);
		PROTOBUF_ENGINE_LIBRARY_API ~AnimFrameBucket();

		PROTOBUF_ENGINE_LIBRARY_API AnimFrameBucket(unsigned int _numNodes);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(AnimFrameBucket_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const AnimFrameBucket_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		unsigned int  keyFrame;
		unsigned int  numNodes;
		float         keyTime;
		int           keyTimeIndex;
		AnimBone* poBone;
	};
}
#endif