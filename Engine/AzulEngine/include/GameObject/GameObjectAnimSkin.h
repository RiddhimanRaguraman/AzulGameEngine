//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKIN_H
#define GAME_OBJECT_ANIM_SKIN_H

#include "GameObjectControlled.h"
#include "ComputeBlend.h"


namespace Azul
{
	class GameObjectAnimSkin : public GameObjectControlled
	{
	public:
		
		GameObjectAnimSkin(	GraphicsObject* graphicsObject, ComputeBlend* pBlend);
		// Big four
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime) override;

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);
		void SetPrefab(Prefab* _poPrefab);

		void SetScale(Vec3 &r);
		void SetQuat(Quat &r);
		void SetTrans(Vec3 &r);

	private:
		void privUpdate(AnimTime currTime);


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

		ComputeBlend* pBlend;
		Prefab* poPrefab;
		bool setorupdate;
		char pad[3];
		
	};
}

#endif

// --- End of File ---
