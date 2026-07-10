//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "GameObjectRigidBody.h"
#include "GameObjectControlled.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectSprite.h"
#include "PCSTree.h"
#include "AnimTime.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	// Singleton
	class AZUL_ENGINE_LIBRARY_API GameObjectMan
	{
	public:
		static void Add(GameObject* pObj, GameObject *pParent);
		static void Draw();
		static void Update(AnimTime currentTime, AnimTime tDelta);

		static GameObject *GetRoot(void);
		static PCSTree *GetPCSTree();

		static void Create();
		static void Destroy();

		// P4.1 A/B toggle: when true, the RenderSystem draws the 3D materials from
		// the component pool and the tree walk draws only the 2D/UI (Sprite);
		// when false, the original PCS-tree walk draws everything.
		static bool GetUseECSRender();
		static void SetUseECSRender(bool b);

		GameObjectMan(const GameObjectMan &) = delete;
		GameObjectMan &operator = (const GameObjectMan &) = delete;

		~GameObjectMan();

	private:
		GameObjectMan();

		static GameObjectMan *privGetInstance();

		// data
		PCSTree *poRootTree;
		static bool sUseECSRender;
	};

}

#endif


// --- End of File ---
