#include "AnimBone.h"

namespace Azul 
{

	AnimBone::AnimBone()
		:S(1, 1, 1),
		Q(0, 0, 0, 1),
		T(1, 1, 1)
	{

	}

	AnimBone::AnimBone(Vec3& s, Quat& q, Vec3& t)
		: S(s),
		Q(q),
		T(t)
	{

	}

	void AnimBone::set(Vec3& s, Quat& q, Vec3& t)
	{
		this->S.set(s);
		this->Q.set(q);
		this->T.set(t);
	}

	void AnimBone::Serialize(AnimBone_proto& out) const
	{
		AnimVec3_proto s_proto;
		this->S.Serialize(s_proto);
		*out.mutable_s() = s_proto;

		AnimQuat_proto q_proto;
		this->Q.Serialize(q_proto);
		*out.mutable_q() = q_proto;

		AnimVec3_proto t_proto;
		this->T.Serialize(t_proto);
		*out.mutable_t() = t_proto;
	}

	void AnimBone::Deserialize(const AnimBone_proto& in)
	{
		this->S.Deserialize(in.s());
		this->Q.Deserialize(in.q());
		this->T.Deserialize(in.t());
	}

	void AnimBone::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);

		Trace::out("{ \n");
		Trace::out("   {%5.5g, %5.5g, %5.5g },        // S\n", S.x, S.y, S.z);
		Trace::out("   {%5.5g, %5.5g, %5.5g, %5.5g }, // Q\n", Q.qx, Q.qy, Q.qz, Q.qw);
		Trace::out("   {%5.5g, %5.5g, %5.5g },        // T\n", T.x, T.y, T.z);
		Trace::out("} \n");
	}
}