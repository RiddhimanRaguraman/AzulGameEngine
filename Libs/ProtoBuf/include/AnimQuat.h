#ifndef QUAT_H
#define QUAT_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "MathEngine.h"
#include "AnimQuat.pb.h"

namespace Azul
{
	class AnimQuat
	{
	public:
		PROTOBUF_ENGINE_LIBRARY_API AnimQuat();
		PROTOBUF_ENGINE_LIBRARY_API AnimQuat(const AnimQuat&) = delete;
		PROTOBUF_ENGINE_LIBRARY_API AnimQuat& operator = (const AnimQuat&) = default;
		PROTOBUF_ENGINE_LIBRARY_API ~AnimQuat();

		PROTOBUF_ENGINE_LIBRARY_API AnimQuat(Quat& r);
		PROTOBUF_ENGINE_LIBRARY_API AnimQuat(float _x, float _y, float _z, float _w);

		PROTOBUF_ENGINE_LIBRARY_API void set(Quat& r);
		PROTOBUF_ENGINE_LIBRARY_API void set(float _x, float _y, float _z, float _w);

		// Serialization 
		PROTOBUF_ENGINE_LIBRARY_API void Serialize(AnimQuat_proto& out) const;
		PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const AnimQuat_proto& in);

		PROTOBUF_ENGINE_LIBRARY_API void Print(const char* const pName) const;

	public:
		float qx;
		float qy;
		float qz;
		float qw;
	};
}
#endif