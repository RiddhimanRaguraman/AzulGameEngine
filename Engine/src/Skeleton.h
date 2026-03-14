//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectControlled.h"
#include "Bone.h"
#include "Skel.h"
#include "JointTable.h"
#include "Clip.h"
namespace Azul
{
	class GameObject;
	class Skeleton
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		Skeleton(Clip::Name _name);
		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		void SetClip(Clip::Name clipName);
		void SetAnimationHierarchy(GameObject *pParent);

		GameObjectControlled* GetFirstBone();
		GameObjectControlled* FindBoneByIndex(int index);
		
		int GetNumNodes() const;
		Clip *GetClip();
		Bone *GetBoneResult();
        void SetPivotScale(float sx, float sy, float sz);
		void SetUniformPivotScale(float s);
		void SetPivotTrans(float x, float y, float z);
		void SetPivotRotX(float angle);
		void SetPivotRotY(float angle);
		void SetPivotRotZ(float angle);
		void SetPivotTotalRot(const Rot3 mode, float x, float y, float z);
    private:
		void privSetAnimationHierarchy(GameObject *pParent);

    private:
        GameObjectControlled *pFirstBone;
		int             mNumNodes;
		Bone *poBoneResult;
		Skel *pSkel;
		Clip *pClip;
    };
}

#endif

// --- End of File ---
