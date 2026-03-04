#ifndef GAME_OBJECT_RIGID_BODY_H
#define GAME_OBJECT_RIGID_BODY_H

#include "GameObject.h"
#include "GraphicsObject.h"

namespace Azul
{
	class GameObjectRigidBody : public GameObject
	{
		friend class Prefab;
	public:
		GameObjectRigidBody(GraphicsObject* pGraphicsObject);
		GameObjectRigidBody() = delete;
		GameObjectRigidBody(const GameObjectRigidBody&) = delete;
		GameObjectRigidBody& operator = (const GameObjectRigidBody&) = delete;
		virtual ~GameObjectRigidBody();

		virtual void Update(AnimTime currentTime) override;

		void SetPos(Vec3 v);
		void SetScale(float v);
		void SetScale(float sx, float sy, float sz);
		void SetTotalRot(const Rot3 mode, float x, float y, float z);
		void SetUniformScale(float s);
		void SetTrans(float x, float y, float z);
		void SetRotX(float x);
		void SetRotY(float y);
		void SetRotZ(float z);
		void SetPrefab(Prefab* _poPrefab);

	private:
		Vec3* poScale;
		Quat* poQuat;
		Vec3* poTrans;
		bool setorupdate;
		char pad[3];
		Prefab* poPrefab;
	};
}
#endif
