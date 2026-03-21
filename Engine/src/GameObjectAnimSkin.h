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
#include "WorldCompute.h"

namespace Azul
{
	class GameObjectAnimSkin : public GameObjectControlled
	{
	public:
		
		GameObjectAnimSkin(	GraphicsObject* graphicsObject, 
							Mixer *pMixer, WorldCompute *pWorldCompute);
		// Big four
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime) override;

		virtual void SetIndex(int i) override;

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(Vec3 &r);
		void SetQuat(Quat &r);
		void SetTrans(Vec3 &r);

	private:
		void privUpdate(AnimTime currTime);
		void privMixerExecute();
		void privWorldComputeExecute();

	public:
		

		Vec3 *poScale;
		Quat *poQuat;
		Vec3 *poTrans;

		float delta_x;
		float delta_y;
		float delta_z;

		float cur_rot_x;
		float cur_rot_y;
		float cur_rot_z;

		Mixer *poMixer;
		WorldCompute *poWorldCompute;
	};
}

#endif

// --- End of File ---
