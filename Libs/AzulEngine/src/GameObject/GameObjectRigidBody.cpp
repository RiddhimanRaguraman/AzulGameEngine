#include "GameObjectRigidBody.h"
#include "Prefab.h"

namespace Azul
{
	GameObjectRigidBody::GameObjectRigidBody(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject),
		poPrefab(nullptr)
	{
		assert(this->GetGraphicsObject());

		// Seed the transform (was poScale/poQuat/poTrans defaults).
		TransformComponent& t = this->GetTransform();
		t.pos.set(0.0f, 0.0f, 0.0f);
		t.scale.set(1.0f, 1.0f, 1.0f);
		t.rot = Quat(Identity);
	}

	GameObjectRigidBody::GameObjectRigidBody(MaterialKind kind)
		: GameObject(kind),
		poPrefab(nullptr)
	{
		// Data-path: no GraphicsObject. Seed the transform defaults.
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

		if (poPrefab != nullptr)
		{
			// prefab drives the world matrix directly
			this->poPrefab->SetData(*this);
			poPrefab->Update();
			poPrefab->ReturnWorld(t.world);
		}
		// non-prefab: world = S*R*T is computed by LocalToWorldSystem before
		// this tree walk (Phase 3).

		// Data-path 3D objects have no GraphicsObject (the RenderSystem reads the
		// world from TransformComponent); only push to a bridge GraphicsObject.
		if (this->GetGraphicsObject() != nullptr)
		{
			this->GetGraphicsObject()->SetWorld(t.world);
		}
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
