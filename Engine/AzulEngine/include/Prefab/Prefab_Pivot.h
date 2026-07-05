#ifndef PREFAB_PIVOT_H
#define PREFAB_PIVOT_H

#include "Prefab.h"

namespace Azul
{
	class Prefab_Pivot : public Prefab
	{
	public:
		Prefab_Pivot();
		virtual void DerivedUpdate() override;

	private:
		float delta_x;
		float delta_y;
		float delta_z;
		float cur_rot_x;
		float cur_rot_y;
		float cur_rot_z;
	};
}

#endif

