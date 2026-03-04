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

namespace Azul
{
	// Abstract Gameobject
	class GameObject : public PCSNode
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

		void DrawEnable();
		void DrawDisable();

	protected:
		Mat4 *poWorld;
		GraphicsObject *poGraphicsObject;
		bool mDrawEnable;

	};

}
#endif


// --- End of File ---
