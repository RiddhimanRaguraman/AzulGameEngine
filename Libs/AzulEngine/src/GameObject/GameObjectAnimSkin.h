//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_SKIN_H
#define GAME_OBJECT_ANIM_SKIN_H

#include "GameObjectControlled.h"
#include "ComputeBlend.h"


#include "EngineDLLInterface.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API GameObjectAnimSkin : public GameObjectControlled
	{
	public:
		
		GameObjectAnimSkin( MaterialKind kind, ComputeBlend* pBlend);
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
		// pos/rot/scale now live in the entity's TransformComponent
		// (see GameObject::GetTransform).

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
