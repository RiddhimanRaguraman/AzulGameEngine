//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Camera.h"
#include "WorldMan.h"
#include "World.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "HierarchyComponent.h"

namespace Azul
{

	GameObject::GameObject(MaterialKind kind)
	{
		// One entity; transform + render + hierarchy in components. The caller
		// fills the render handles via GetRender().
		this->mEntity = WorldMan::GetWorld().Create();

		TransformComponent &t = WorldMan::GetWorld().Add<TransformComponent>(this->mEntity);
		t.world = Mat4(Identity);

		RenderComponent &r = WorldMan::GetWorld().Add<RenderComponent>(this->mEntity);
		r.kind = kind;
		r.drawEnable = true;
		r.layer = 0;
		r.pMesh = nullptr;
		r.pShader = nullptr;
		r.pTex = nullptr;
		r.pComputeBlend = nullptr;
		r.lightColor.set(0.0f, 0.0f, 0.0f);
		r.lightPos.set(0.0f, 0.0f, 0.0f);
		r.bodyColor.set(0.0f, 0.0f, 0.0f);
		r.uvMatrix = Mat4(Identity);

		HierarchyComponent &h = WorldMan::GetWorld().Add<HierarchyComponent>(this->mEntity);
		h.parent = EntityNull();
	}

	GameObject::~GameObject()
	{
		WorldMan::GetWorld().Destroy(this->mEntity);
	}

	TransformComponent &GameObject::GetTransform()
	{
		TransformComponent *pT = WorldMan::GetWorld().TryGet<TransformComponent>(this->mEntity);
		assert(pT);
		return *pT;
	}

	RenderComponent &GameObject::GetRender()
	{
		RenderComponent *pR = WorldMan::GetWorld().TryGet<RenderComponent>(this->mEntity);
		assert(pR);
		return *pR;
	}

	HierarchyComponent &GameObject::GetHierarchy()
	{
		HierarchyComponent *pH = WorldMan::GetWorld().TryGet<HierarchyComponent>(this->mEntity);
		assert(pH);
		return *pH;
	}

	Entity GameObject::GetEntity() const
	{
		return this->mEntity;
	}

	void GameObject::SetParent(GameObject *pParent)
	{
		assert(pParent);
		this->GetHierarchy().parent = pParent->GetEntity();
	}

	Mat4 *GameObject::GetWorld()
	{
		return &this->GetTransform().world;
	}

	void GameObject::SetWorld(Mat4 *pWorld)
	{
		assert(pWorld);
		this->GetTransform().world = *pWorld;
	}

	void GameObject::DrawEnable()
	{
		this->GetRender().drawEnable = true;
	}

	void GameObject::DrawDisable()
	{
		this->GetRender().drawEnable = false;
	}

	void GameObject::Draw()
	{
		// P5.1: draw is fully pool-driven (RenderSystem::Draw for 3D, then
		// SpriteRenderSystem::Draw for 2D/text) -- GameObjectMan::Draw no longer
		// walks the tree, so this base no-op is now unused. Kept until the
		// GameObject shim itself is removed (P5.4).
	}

}

// --- End of File ---
