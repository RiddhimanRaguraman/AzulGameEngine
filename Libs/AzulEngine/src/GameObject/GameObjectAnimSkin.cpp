//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

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
#include "WorldMan.h"
#include "World.h"
#include "GpuSkinComponent.h"

namespace Azul
{

	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *pGraphicsObject, ComputeBlend* _pBlend)
		: GameObjectControlled(pGraphicsObject),
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
		assert(this->pBlend);

		// Seed the transform (was poScale/poQuat/poTrans defaults).
		TransformComponent& t = this->GetTransform();
		t.pos.set(0.0f, 0.0f, 0.0f);
		t.scale.set(1.0f, 1.0f, 1.0f);
		t.rot.set(0.0f, 0.0f, 0.0f, 1.0f);

		// Phase 3: the GPU compute-skinning dispatch is now data-driven -- the
		// SkinningSystem calls pBlend->Execute() over this pool each frame
		// (non-owning handle; the AnimController still owns the ComputeBlend).
		GpuSkinComponent& skin = WorldMan::GetWorld().Add<GpuSkinComponent>(this->GetEntity());
		skin.pBlend = this->pBlend;
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete this->poPrefab;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		if (setorupdate == true && poPrefab != nullptr)
		{
			this->poPrefab->SetData(*this);
			poPrefab->Update();
			poPrefab->ReturnWorld(*this->GetWorld());
		}
		else
		{
			this->privUpdate(currentTime);
			setorupdate = true;
		}

		// update the bounding volume based on world matrix
		this->GetGraphicsObject()->SetWorld(*this->GetWorld());

		// NOTE: the GPU compute-skinning dispatch (pBlend->Execute) now runs in
		// the SkinningSystem over the GpuSkinComponent pool -- see GameObjectMan.
	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		TransformComponent& t = this->GetTransform();
		Trans T(t.pos);
		Scale S(t.scale);
		Quat  Q(t.rot);

		cur_rot_x += delta_x;
		cur_rot_y += delta_y;
		cur_rot_z += delta_z;

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		// world matrix
		t.world = S * Q * Rx * Ry * Rz * T;
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->GetTransform().rot.set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		this->GetTransform().scale.set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		this->GetTransform().pos.set(x, y, z);
	}

	void GameObjectAnimSkin::SetPrefab(Prefab* _poPrefab)
	{
		this->poPrefab = _poPrefab;
	}

	void GameObjectAnimSkin::SetScale(Vec3 &r)
	{
		this->GetTransform().scale = r;
	}

	void GameObjectAnimSkin::SetQuat(Quat &r)
	{
		this->GetTransform().rot = r;
	}

	void GameObjectAnimSkin::SetTrans(Vec3 &r)
	{
		this->GetTransform().pos = r;
	}
}

// --- End of File ---
