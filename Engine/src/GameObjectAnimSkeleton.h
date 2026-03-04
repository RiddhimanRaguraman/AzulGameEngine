//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKELETON_H
#define GAME_OBJECT_ANIM_SKELETON_H

#include "GameObjectControlled.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"
#include "Bone.h"

namespace Azul
{
	class GameObjectAnimSkeleton : public GameObjectControlled
	{
	public:
		GameObjectAnimSkeleton(GraphicsObject *graphicsObject, Bone *pBoneResult);

		// Big four
		GameObjectAnimSkeleton() = delete;
		GameObjectAnimSkeleton(const GameObjectAnimSkeleton &) = delete;
		GameObjectAnimSkeleton &operator=(GameObjectAnimSkeleton &) = delete;
		virtual ~GameObjectAnimSkeleton();

		virtual void Update(AnimTime currTime);

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(Vec3 &r);
		void SetQuat(Quat &r);
		void SetTrans(Vec3 &r);

		virtual void SetIndex(int i) override;


	private:
		void privUpdate(AnimTime currTime);

	public:   // add accessors later
		Vec3 *poScale;
		Quat *poQuat;
		Vec3 *poTrans;
		Mat4 *poLocal;
		Bone *pBoneResult;

	};
}

#endif

// --- End of File ---
