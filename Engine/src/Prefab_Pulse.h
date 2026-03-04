#ifndef Prefab_Pulse_H
#define Prefab_Pulse_H

#include "Prefab.h"

namespace Azul 
{
	class Prefab_Pulse : public Prefab
	{
	public:
		Prefab_Pulse() = default;
		virtual void DerivedUpdate() override;
	};
}

#endif