//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKELETON_H
#define SKELETON_H

#include "Bone.h"
#include "Skel.h"
#include "Clip.h"
namespace Azul
{
	class GameObject;
	class Skeleton
	{

	public:
		Skeleton(Clip::Name _name);
		Skeleton() = delete;
		Skeleton(const Skeleton &) = delete;
		Skeleton &operator = (const Skeleton &) = delete;
		~Skeleton();

		void SetClip(Clip::Name clipName);
		
		int GetNumNodes() const;
		Clip *GetClip();
		Bone *GetBoneResult();


    private:
		int             mNumNodes;
		Bone *poBoneResult;
		Skel *pSkel;
		Clip *pClip;
    };
}

#endif

// --- End of File ---
