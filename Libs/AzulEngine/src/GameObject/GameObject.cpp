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

namespace Azul
{

	GameObject::GameObject(GraphicsObject *pGraphicsObject)
	{
		assert(pGraphicsObject);

		// Bridge: one entity, its transform + render data live in components.
		this->mEntity = WorldMan::GetWorld().Create();

		TransformComponent &t = WorldMan::GetWorld().Add<TransformComponent>(this->mEntity);
		t.world = Mat4(Identity);

		RenderComponent &r = WorldMan::GetWorld().Add<RenderComponent>(this->mEntity);
		r.pGraphicsObject = pGraphicsObject;
		r.drawEnable = true;
	}

	GameObject::~GameObject()
	{
		// The GameObject owns its GraphicsObject; free it before the entity
		// (and its components) go away.
		delete this->GetRender().pGraphicsObject;
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

	Mat4 *GameObject::GetWorld()
	{
		return &this->GetTransform().world;
	}

	GraphicsObject *GameObject::GetGraphicsObject()
	{
		return this->GetRender().pGraphicsObject;
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
		assert(r.pGraphicsObject);
		if (r.drawEnable)
		{
			r.pGraphicsObject->Render();
		}
	}

}

// --- End of File ---
