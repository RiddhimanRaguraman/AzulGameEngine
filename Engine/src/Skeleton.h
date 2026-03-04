//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectControlled.h"
#include "Bone.h"
#include "Skel.h"
#include "JointTable.h"
namespace Azul
{
    class GameObjectRigidBody;
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
		Skeleton(Bone* pBone, unsigned int _numBones, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable);

		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		GameObjectControlled* GetFirstBone();
		GameObjectControlled* FindBoneByIndex(int index);
		
		int GetNumBones() const;

        void SetPivotScale(float sx, float sy, float sz);
		void SetUniformPivotScale(float s);
		void SetPivotTrans(float x, float y, float z);
		void SetPivotRotX(float angle);
		void SetPivotRotY(float angle);
		void SetPivotRotZ(float angle);
		void SetPivotTotalRot(const Rot3 mode, float x, float y, float z);
    private:
		void privSetAnimationHierarchy(Bone* pBoneResult, Skel::Name skelName, TextureObject::Name texName, Mesh::Name meshName, JointTable* pJointTable);

    private:
        GameObjectControlled *pFirstBone;
		GameObjectRigidBody *pPivot;
        int             numBones;
    };
}

#endif

// --- End of File ---
