#include "Prefab_RoatateSpin.h"

namespace Azul
{
	void Prefab_RotateSpin::DerivedUpdate()
	{
		//float scale = 1.0f;
		float trans = 0.0f;
		static float angle = 0.0f;
		angle += 0.01f;

		Trans TransC(poTrans->x(), poTrans->y(), poTrans->z());
		Scale ScaleA(poScale->x(), poScale->y(), poScale->z());
		Trans TransA(trans, 0.0f, -4.0f);
		Trans TransB(Trig::sin(2.1f * angle) * 0.5f,
			Trig::cos(1.7f * angle) * 0.5f,
			Trig::sin(1.3f * angle) * Trig::cos(1.5f * angle) * 2.0f);
		Rot RotY(Rot1::Y, 0.8f * angle);
		Rot RotX(Rot1::X, 0.4f * angle);

		*this->poWorld = ScaleA * TransC *  RotX * RotY * TransB * TransA;

	}
}
