//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "Prefab.h"
#include "GraphicsObject_ColorByVertex.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_Null.h"
#include "GraphicsObject_Sprite.h"
#include "AnimTime.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	// Abstract Gameobject
	class AZUL_ENGINE_LIBRARY_API GameObject : public PCSNode
	{
	public:
		GameObject() = delete;
		GameObject(const GameObject &) = delete;
		GameObject &operator = (const GameObject &) = delete;
		virtual ~GameObject();

		GameObject(GraphicsObject *poGraphicsObject);

		virtual void Update(AnimTime currentTime) = 0;
		virtual void Draw();

	// Set/Get
		GraphicsObject *GetGraphicsObject();
		Mat4 *GetWorld();
		void SetWorld(Mat4 *pWorld);

		// The entity's transform (pos/rot/scale/world). Subclasses and Prefab
		// read/write pos/rot/scale here instead of owning Vec3*/Quat* members.
		TransformComponent &GetTransform();

		void DrawEnable();
		void DrawDisable();

	protected:
		RenderComponent &GetRender();

		// Bridge: the transform and render data now live in ECS components keyed
		// by this entity. The GameObject is a thin shim holding only the id.
		Entity mEntity;

	};

}
#endif


// --- End of File ---
