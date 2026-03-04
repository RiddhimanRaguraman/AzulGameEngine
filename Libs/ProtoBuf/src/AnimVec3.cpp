#include "AnimVec3.h"

namespace Azul
{
	AnimVec3::~AnimVec3()
	{

	}

	AnimVec3::AnimVec3()
		:x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	AnimVec3::AnimVec3(Vec3& r)
		:x(r.x()),
		y(r.y()),
		z(r.z())
	{

	}

	AnimVec3::AnimVec3(float _x, float _y, float _z)
		:x(_x),
		y(_y),
		z(_z)
	{

	}


	void AnimVec3::set(Vec3& r)
	{
		this->x = r.x();
		this->y = r.y();
		this->z = r.z();
	}

	void AnimVec3::set(float _x, float _y, float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	void AnimVec3::Serialize(AnimVec3_proto& out) const
	{
		AZUL_UNUSED_VAR(out);
		out.set_x(this->x);
		out.set_y(this->y);
		out.set_z(this->z);
	}

	void AnimVec3::Deserialize(const AnimVec3_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		this->x = in.x();
		this->y = in.y();
		this->z = in.z();
	}

	void AnimVec3::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);

		Trace::out("{%f, %f, %f },\n", this->x, this->y, this->z);
	}
}

