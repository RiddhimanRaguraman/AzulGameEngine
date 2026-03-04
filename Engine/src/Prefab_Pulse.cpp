#include "Prefab_Pulse.h"

namespace Azul
{
	void Prefab_Pulse::DerivedUpdate()
	{
		
		//Scale pulse
		static float t = 0.0f;
		t += 0.04f;

		// pulse between 0.75 and 1.25
		float s = 1.0f + 0.25f * Trig::cos(t);

		Trans move(poTrans->x(), poTrans->y(), poTrans->z());
		Scale _scale(s, s, s);

		*this->poWorld = move * _scale;
	}
}

