#ifndef PREFAB_H
#define PREFAB_H

#include "Prefab_Abstract.h"

namespace Azul
{
	class GameObjectRigidBody;
	class GameObjectAnimSkin;

	class Prefab : public Prefab_Abstract
	{
	public:
		Prefab();
		Prefab(GameObjectRigidBody *pGom);
		Prefab(const Prefab&) = delete;
		virtual ~Prefab();
		void SetData(GameObjectRigidBody& pGom);
		void SetData(GameObjectAnimSkin& pGom);
		void SetScale(float sx, float sy, float sz);
		void SetTrans(float x, float y, float z);
		virtual void Update() override;

		void ReturnWorld(Mat4& input);

	protected:
		virtual void DerivedUpdate() = 0;
		Vec3* poScale;
		Quat* poQuat;
		Vec3* poTrans;
		Mat4* poWorld;
	};
}

#endif // !PREFAB_H
