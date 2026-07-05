#include "Prefab_Pivot.h"

namespace Azul
{
	Prefab_Pivot::Prefab_Pivot()
		: delta_x(0.003f),
		delta_y(0.0f),
		delta_z(0.f),
		cur_rot_x(0.0f),
		cur_rot_y(0.0f),
		cur_rot_z(0.0f)
	{
	}

	void Prefab_Pivot::DerivedUpdate()
	{
		cur_rot_x += delta_x;
		cur_rot_y += delta_y;
		cur_rot_z += delta_z;

		Trans T(poTrans->x(), poTrans->y(), poTrans->z());
		Scale S(poScale->x(), poScale->y(), poScale->z());

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		Rot Rbase(*this->poQuat);

		*this->poWorld = S * Rbase * Rx * Ry * Rz * T;
	}
}
