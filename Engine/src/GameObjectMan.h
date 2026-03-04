//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "GameObjectRigidBody.h"
#include "GameObjectControlled.h"
#include "GameObjectAnimSkin.h"
#include "GameObjectAnimSkeleton.h"
#include "GameObjectSprite.h"
#include "PCSTree.h"
#include "AnimTime.h"

namespace Azul
{
	// Singleton
	class GameObjectMan
	{
	public:
		static void Add(GameObject* pObj, GameObject *pParent);
		static void Draw();
		static void Update(AnimTime currentTime);

		static GameObject *GetRoot(void);
		static PCSTree *GetPCSTree();

		static void Create();
		static void Destroy();

		GameObjectMan(const GameObjectMan &) = delete;
		GameObjectMan &operator = (const GameObjectMan &) = delete;

		~GameObjectMan();

	private:
		GameObjectMan();

		static GameObjectMan *privGetInstance();

		// data
		PCSTree *poRootTree;
	};

}

#endif


// --- End of File ---
