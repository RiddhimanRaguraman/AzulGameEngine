#include "Prefab_Pendulum.h"

namespace Azul
{
	void Prefab_Pendulum::DerivedUpdate()
	{
		static float t = 0.0f;
		t += 0.03f;

		// amplitude of swing
		float dx = Trig::sin(t) * 2.0f;

		Trans base(poTrans->x(), poTrans->y(), poTrans->z());
		Trans swing(dx, 0.0f, 0.0f);
		Scale scaleA(poScale->x(), poScale->y(), poScale->z());

		*this->poWorld = scaleA * base * swing ;
	}
}
