#ifndef VEC3_H
#define VEC3_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "MathEngine.h"
#include "AnimVec3.pb.h"

namespace Azul
{

	class AnimVec3
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API AnimVec3();
		PROTOBUF_ENGINE_LIBRARY_API AnimVec3(const AnimVec3&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API AnimVec3& operator = (const AnimVec3&) = default;
		PROTOBUF_ENGINE_LIBRARY_API ~AnimVec3();

		PROTOBUF_ENGINE_LIBRARY_API AnimVec3(Vec3& r);
		PROTOBUF_ENGINE_LIBRARY_API AnimVec3(float _x, float _y, float _z);

		PROTOBUF_ENGINE_LIBRARY_API void set(Vec3& r);
		PROTOBUF_ENGINE_LIBRARY_API void set(float _x, float _y, float _z);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(AnimVec3_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const AnimVec3_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		float x;
		float y;
		float z;
	};
}
#endif
