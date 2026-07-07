#include "GameObjectRigidBody.h"
#include "Prefab.h"

namespace Azul
{
	GameObjectRigidBody::GameObjectRigidBody(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject),
		setorupdate(false),
		poPrefab(nullptr)
	{
		assert(this->GetGraphicsObject());

		// Seed the transform (was poScale/poQuat/poTrans defaults).
		TransformComponent& t = this->GetTransform();
		t.pos.set(0.0f, 0.0f, 0.0f);
		t.scale.set(1.0f, 1.0f, 1.0f);
		t.rot = Quat(Identity);
	}

	GameObjectRigidBody::~GameObjectRigidBody()
	{
		delete this->poPrefab;
	}

	void GameObjectRigidBody::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		TransformComponent& t = this->GetTransform();

		if (setorupdate == true && poPrefab != nullptr)
		{
			this->poPrefab->SetData(*this);
			poPrefab->Update();
			poPrefab->ReturnWorld(t.world);
		}
		else
		{
			Scale ScaleA(t.scale.x(), t.scale.y(), t.scale.z());
			Rot RotA(t.rot);
			Trans TransA(t.pos.x(), t.pos.y(), t.pos.z());

			t.world = ScaleA * RotA * TransA;
			setorupdate = true;
		}

		this->GetGraphicsObject()->SetWorld(t.world);
	}

	void GameObjectRigidBody::SetPos(Vec3 v)
	{
		this->GetTransform().pos.set(v);
	}

	void GameObjectRigidBody::SetTrans(float x, float y, float z)
	{
		this->GetTransform().pos.set(x, y, z);
	}

	void GameObjectRigidBody::SetScale(float v)
	{
		this->GetTransform().scale.set(v, v, v);
	}

	void GameObjectRigidBody::SetScale(float sx, float sy, float sz)
	{
		this->GetTransform().scale.set(sx, sy, sz);
	}

	void GameObjectRigidBody::SetTotalRot(const Rot3 mode, float x, float y, float z)
	{
		Rot R(mode, x, y, z);
		this->GetTransform().rot.set(R);
	}

	void GameObjectRigidBody::SetUniformScale(float s)
	{
		this->SetScale(s, s, s);
	}

	void GameObjectRigidBody::SetRotX(float x)
	{
		Rot R(Rot1::X, x);
		this->GetTransform().rot.set(R);
	}

	void GameObjectRigidBody::SetRotY(float y)
	{
		Rot R(Rot1::Y, y);
		this->GetTransform().rot.set(R);
	}

	void GameObjectRigidBody::SetRotZ(float z)
	{
		Rot R(Rot1::Z, z);
		this->GetTransform().rot.set(R);
	}

	void GameObjectRigidBody::SetPrefab(Prefab* _poPrefab)
	{
		this->poPrefab = _poPrefab;
	}

}
