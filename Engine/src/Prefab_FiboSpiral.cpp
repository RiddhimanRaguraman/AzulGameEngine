#include "Prefab_FiboSpiral.h"
#include "MathEngine.h"

namespace Azul
{
    void Prefab_FiboSpiral::DerivedUpdate()
    {
        // advance by a constant step each frame
        this->angle += ANGLE_INC;

        // radius grows proportional to angle
        float radius = this->rGrowth * this->angle;

        // each turn offset by golden angle
        float theta = this->angle * GOLDEN_ANGLE;

        // compute offset in XZ plane (not XY)
        float x = radius * Trig::cos(theta);
        float z = radius * Trig::sin(theta);

        Trans move(poTrans->x() + x, poTrans->y(), poTrans->z() + z);
        Scale scaleA(poScale->x(), poScale->y(), poScale->z());
        *this->poWorld = scaleA * move;
    }
}
