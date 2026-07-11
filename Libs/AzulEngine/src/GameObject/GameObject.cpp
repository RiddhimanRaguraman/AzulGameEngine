//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "RenderSystem.h"
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
		// ---------------------------------------------
		//  Transfer data to the constant buffer
		//    CPU ---> GPU
		//    World, View, Projection Matrix
		//    Set Shader
		//    Render
		// ---------------------------------------------

		RenderComponent &r = this->GetRender();
		if (!r.drawEnable)
		{
			return;
		}

		// 3D materials render through the RenderSystem's per-MaterialKind branches,
		// driven here in the tree walk so they keep the authoritative PCS-tree draw
		// order (correct alpha-blend layering). 2D/UI sprites override Draw
		// (GameObjectSprite/FontSprite). kind==Null (the root) draws nothing.
		if (MaterialKindIs3D(r.kind))
		{
			RenderSystem::DrawObject(r, this->GetTransform().world);
		}
	}

}

// --- End of File ---
