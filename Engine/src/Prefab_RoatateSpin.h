#ifndef PREFAB_ROTATE_SPIN_H
#define PREFAB_ROTATE_SPIN_H

#include "Prefab.h"

namespace Azul
{
	class Prefab_RotateSpin : public Prefab
	{
	public:
		Prefab_RotateSpin() = default;
		virtual void DerivedUpdate() override;
	};
}

#endif