//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "ShaderMappings.h"
#include "Mesh.h"
#include "Anim.h"
#include "ShaderObjectNodeMan.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{

	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *pGraphicsObject, ComputeBlend* _pBlend)
		: GameObjectControlled(pGraphicsObject),
		poScale{ new Vec3(1.0f, 1.0f, 1.0f) },
		poQuat{ new Quat(0.0f, 0.0f, 0.0f, 1.0f) },
		poTrans{ new Vec3(0.0f, 0.0f, 0.0f) },
		delta_x{ 0.0f },
		delta_y{ 0.0f },
		delta_z{ 0.0f },
		cur_rot_x{ 0.0f },
		cur_rot_y{ 0.0f },
		cur_rot_z{ 0.0f },
		pBlend{ _pBlend },
		setorupdate(false),
		poPrefab(nullptr)
	{
		assert(pGraphicsObject);

		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);

		assert(this->pBlend);
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete this->poPrefab;
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		if (setorupdate == true && poPrefab != nullptr)
		{
			this->poPrefab->SetData(*this);
			poPrefab->Update();
			poPrefab->ReturnWorld(*this->poWorld);
		}
		else
		{
			this->privUpdate(currentTime);
			setorupdate = true;
		}

		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);

		// do the compute shaders
		this->pBlend->Execute();
	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		Trans T(*this->poTrans);
		Scale S(*this->poScale);
		Quat  Q(*this->poQuat);

		cur_rot_x += delta_x;
		cur_rot_y += delta_y;
		cur_rot_z += delta_z;

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		// world matrix
		*this->poWorld = S * Q * Rx * Ry * Rz * T;
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnimSkin::SetPrefab(Prefab* _poPrefab)
	{
		this->poPrefab = _poPrefab;
	}

	void GameObjectAnimSkin::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void GameObjectAnimSkin::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnimSkin::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}
}

// --- End of File ---
