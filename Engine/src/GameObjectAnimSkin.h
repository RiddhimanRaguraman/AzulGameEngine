//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKIN_H
#define GAME_OBJECT_ANIM_SKIN_H

#include "GameObjectControlled.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
#include "Bone.h"
#include "Skeleton.h"

namespace Azul
{
	class GameObjectAnimSkin : public GameObjectControlled
	{
	public:
		
		GameObjectAnimSkin(GraphicsObject* graphicsObject, Skeleton* pSkeleton, JointTable* pJointTable);
		// Big four
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);

		virtual void SetIndex(int i) override;

	private:
		void privUpdateBoneWorldArray();

	public:
		Mat4* poBoneWorld;
		Skeleton* pSkeleton;
		JointTable* pJointTable;
	};
}

#endif

// --- End of File ---
