#ifndef PREFAB_FIBOSPIRAL_H
#define PREFAB_FIBOSPIRAL_H

#include "Prefab.h"

namespace Azul
{
    class Prefab_FiboSpiral : public Prefab
    {
    public:
        Prefab_FiboSpiral(float growth = 0.05f)
            : angle(0.0f), rGrowth(growth) {
        }
        virtual void DerivedUpdate() override;

    private:
        float angle;                  // current “time” along the spiral
        float rGrowth;                // how quickly radius grows per radian
        static constexpr float GOLDEN_ANGLE = 2.39996322973f; // ~137.5°
        static constexpr float ANGLE_INC = 0.02f;
    };
}

#endif // PREFAB_FIBOSPIRAL_H
