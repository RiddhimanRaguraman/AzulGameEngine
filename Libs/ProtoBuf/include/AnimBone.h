#ifndef BONE_PROTOBUF_H
#define BONE_PROTOBUF_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"

#include "MathEngine.h"
#include "AnimBone.pb.h"
#include "AnimVec3.h"
#include "AnimQuat.h"

namespace Azul{
    class AnimBone
    {
    public:
        PROTOBUF_ENGINE_LIBRARY_API AnimBone();
        PROTOBUF_ENGINE_LIBRARY_API AnimBone(const AnimBone&) = delete;
        PROTOBUF_ENGINE_LIBRARY_API AnimBone& operator = (const AnimBone&) = default;
        PROTOBUF_ENGINE_LIBRARY_API ~AnimBone() = default;

        PROTOBUF_ENGINE_LIBRARY_API AnimBone(Vec3& s, Quat& q, Vec3& t);
        PROTOBUF_ENGINE_LIBRARY_API void set(Vec3& s, Quat& q, Vec3& t);

        PROTOBUF_ENGINE_LIBRARY_API void Serialize(AnimBone_proto& out) const;
        PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const AnimBone_proto& in);

        PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

    public:
        AnimVec3 S;
        AnimQuat Q;
        AnimVec3 T;
    };
}


#endif