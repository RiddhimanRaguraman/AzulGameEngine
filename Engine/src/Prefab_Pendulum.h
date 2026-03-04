#ifndef PREFAB_PENDULUM_H
#define PREFAB_PENDULUM_H

#include "Prefab.h"

namespace Azul
{
	class Prefab_Pendulum : public Prefab
	{
	public:
		Prefab_Pendulum() = default;
		virtual void DerivedUpdate() override;
	};
}

#endif