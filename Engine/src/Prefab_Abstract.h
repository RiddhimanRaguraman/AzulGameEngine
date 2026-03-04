#ifndef PREFAB_ABSTRACT_H
#define PREFAB_ABSTRACT_H

#include "MathEngine.h"

namespace Azul
{
	class Prefab_Abstract
	{
	protected:
		Prefab_Abstract() = default;
		Prefab_Abstract(const Prefab_Abstract&) = delete;
		Prefab_Abstract& operator = (const Prefab_Abstract&) = delete;
		virtual ~Prefab_Abstract() = default;

		virtual void Update() = 0;
		/*{
			assert(false,"Set up the prefab this is reachhing abstract no derived is called ");
		}*/

	};
}
#endif