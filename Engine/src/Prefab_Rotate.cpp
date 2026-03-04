#include "Prefab_Rotate.h"

namespace Azul
{
	void Prefab_Rotate::DerivedUpdate()
    {
        static float angle = 0.0f;
        angle += 0.01f;

        Trans TransA(poTrans->x(), poTrans->y(), poTrans->z());
        Scale ScaleA(poScale->x(), poScale->y(), poScale->z());
        Mat4 baseRot(Identity);
        if (poQuat != nullptr)
        {
            Rot Rbase(*poQuat);
            baseRot = Rbase;
        }

        Rot Rspin(Rot1::Z, angle);

        *this->poWorld = ScaleA * baseRot * Rspin * TransA;
    }
}

