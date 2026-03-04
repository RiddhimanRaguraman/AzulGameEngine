#include "Prefab.h"
#include "GameObjectRigidBody.h"

namespace Azul {
	Prefab::Prefab()
		: poScale(new Vec3(1.0f, 1.0f, 1.0f)),
		poQuat(new Quat(Identity)),
		poTrans(new Vec3(0.0f, 0.0f, 0.0f))
	{
		assert(poScale);
		assert(poQuat);
		assert(poTrans);

		this->poWorld = new Mat4(Identity);
		assert(this->poWorld);
	}

	Prefab::Prefab(GameObjectRigidBody* pGom)
		: poScale(new Vec3(*pGom->poScale)),
		poQuat(new Quat(*pGom->poQuat)),
		poTrans(new Vec3(*pGom->poTrans))
	{
		assert(poScale);
		assert(poQuat);
		assert(poTrans);

		this->poWorld = new Mat4(Identity);
		assert(this->poWorld);
	}

	Prefab::~Prefab()
	{
		delete this->poScale;
		delete this->poQuat;
		delete this->poTrans;
		delete this->poWorld;
	}

	//void Prefab::SetData(GameObject& pGom)
	//{
	//	this->scale = pGom.scale;
	//	this->XAngle = pGom.XAngle;
	//	this->YAngle = pGom.YAngle;
	//	this->ZAngle = pGom.ZAngle;
	//	this->Mode = pGom.Mode;
	//	this->poPos->set(*pGom.poPos);
	//}

	void Prefab::SetData(GameObjectRigidBody& pGom)
	{
		this->poScale->set(*pGom.poScale);
		*this->poQuat = *pGom.poQuat;
		this->poTrans->set(*pGom.poTrans);
	}

	void Prefab::Update()
	{
		this->DerivedUpdate();
	}
	void Prefab::ReturnWorld(Mat4& input)
	{
		input = *this->poWorld;
	}
}
