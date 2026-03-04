#include "GameObjectRigidBody.h"
#include "Prefab.h"

namespace Azul
{
	GameObjectRigidBody::GameObjectRigidBody(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject),
		poScale(new Vec3(1.0f, 1.0f, 1.0f)),
		poQuat(new Quat(Identity)),
		poTrans(new Vec3(0.0f, 0.0f, 0.0f)),
		setorupdate(false),
		poPrefab(nullptr)
	{
		assert(poGraphicsObject);
		assert(poScale);
		assert(poQuat);
		assert(poTrans);
	}

	GameObjectRigidBody::~GameObjectRigidBody()
	{
		delete this->poPrefab;
		delete this->poScale;
		delete this->poQuat;
		delete this->poTrans;
	}

	void GameObjectRigidBody::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		if (setorupdate == true && poPrefab != nullptr)
		{
			this->poPrefab->SetData(*this);
			poPrefab->Update();
			poPrefab->ReturnWorld(*this->poWorld);
		}
		else
		{
			Scale ScaleA(poScale->x(), poScale->y(), poScale->z());
			Rot RotA(*poQuat);
			Trans TransA(poTrans->x(), poTrans->y(), poTrans->z());

			*this->poWorld = ScaleA * RotA * TransA;
			setorupdate = true;
		}

		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObjectRigidBody::SetPos(Vec3 v)
	{
		poTrans->set(v);
	}

	void GameObjectRigidBody::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}
	
	void GameObjectRigidBody::SetScale(float v)
	{
		this->poScale->set(v, v, v);
	}

	void GameObjectRigidBody::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectRigidBody::SetTotalRot(const Rot3 mode, float x, float y, float z)
	{
		Rot R(mode, x, y, z);
		this->poQuat->set(R);
	}

	void GameObjectRigidBody::SetUniformScale(float s)
	{
		this->SetScale(s, s, s);
	}

	

	void GameObjectRigidBody::SetRotX(float x)
	{
		Rot R(Rot1::X, x);
		this->poQuat->set(R);
	}

	void GameObjectRigidBody::SetRotY(float y)
	{
		Rot R(Rot1::Y, y);
		this->poQuat->set(R);
	}

	void GameObjectRigidBody::SetRotZ(float z)
	{
		Rot R(Rot1::Z, z);
		this->poQuat->set(R);
	}

	void GameObjectRigidBody::SetPrefab(Prefab* _poPrefab)
	{
		this->poPrefab = _poPrefab;
	}

}
