#ifndef PREFAB_ROTATE_H
#define PREFAB_ROTATE_H

#include "Prefab.h"

namespace Azul 
{
	class Prefab_Rotate : public Prefab
	{
	public:
		Prefab_Rotate() = default;
		virtual void DerivedUpdate() override;
	};
}

#endif