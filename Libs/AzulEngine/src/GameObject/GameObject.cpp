//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObject.h"
#include "Camera.h"
#include "WorldMan.h"
#include "World.h"
#include "TransformComponent.h"

namespace Azul
{

	GameObject::GameObject(GraphicsObject *pGraphicsObject)
		: poGraphicsObject{pGraphicsObject},
		mDrawEnable{true}
	{
		assert(pGraphicsObject);

		// Bridge: allocate an entity and back the world transform with a
		// TransformComponent (initialised to Identity, as poWorld was).
		this->mEntity = WorldMan::GetWorld().Create();
		TransformComponent &t = WorldMan::GetWorld().Add<TransformComponent>(this->mEntity);
		t.world = Mat4(Identity);
	}

	GameObject::~GameObject()
	{
		WorldMan::GetWorld().Destroy(this->mEntity);
		delete this->poGraphicsObject;
	}

	Mat4 *GameObject::GetWorld()
	{
		TransformComponent *pT = WorldMan::GetWorld().TryGet<TransformComponent>(this->mEntity);
		assert(pT);
		return &pT->world;
	}

	GraphicsObject *GameObject::GetGraphicsObject()
	{
		return this->poGraphicsObject;
	}

	void GameObject::SetWorld(Mat4 *pWorld)
	{
		assert(pWorld);
		TransformComponent *pT = WorldMan::GetWorld().TryGet<TransformComponent>(this->mEntity);
		assert(pT);
		pT->world = *pWorld;
	}

	void GameObject::DrawEnable()
	{
		this->mDrawEnable = true;
	}

	void GameObject::DrawDisable()
	{
		this->mDrawEnable = false;
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

		GraphicsObject *pGraphicsObj;
		pGraphicsObj = this->GetGraphicsObject();
		assert(pGraphicsObj);
		if(this->mDrawEnable)
		{
		this->poGraphicsObject->Render();
		}
	}

}

// --- End of File ---
