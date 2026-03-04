#ifndef CLIP_PROTOBUF_H
#define CLIP_PROTOBUF_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "AnimClip.pb.h"
#include "AnimFrameBucket.h"

namespace Azul 
{
    class AnimClip
    {
    public:
		PROTOBUF_ENGINE_LIBRARY_API AnimClip();
		PROTOBUF_ENGINE_LIBRARY_API AnimClip(const AnimClip&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API AnimClip& operator = (const AnimClip&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API ~AnimClip();

		PROTOBUF_ENGINE_LIBRARY_API AnimClip(unsigned int _numKeyFrames, unsigned int _numBones);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(AnimClip_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const AnimClip_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		char pVersion[protoVersion::VERSION_NUM_BYTES];
		unsigned int numKeyFrames;
		unsigned int numBones;
        AnimFrameBucket* poFrameBucketEntry;
    };
}
#endif