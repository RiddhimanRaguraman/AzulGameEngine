//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_MANAGER_H
#define CLIP_MANAGER_H

#include "ManBase.h"
#include "Clip.h"
#include "AnimClip.h"
#include "HierarchyTable.h"

namespace Azul
{
	class ClipMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static Clip *Find(Clip::Name name);

		static void Remove(Clip *pNode);
		static void Dump();

		static void Add(Clip::Name clipName, const char* const pFileName, Skel::Name skelName, HierarchyTable::Name hierarchyName);
		static void Add(Clip::Name clipName,
			unsigned int numNodes,
			unsigned int numKeyFrames,
			AnimFrameBucket* pFrameBucket,
			Skel::Name skelName,
			HierarchyTable::Name hierarchyName);


		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static ClipMan *privGetInstance();

		ClipMan() = delete;
		ClipMan(const ClipMan &) = delete;
		ClipMan &operator=(const ClipMan &) = delete;
		virtual ~ClipMan();

		ClipMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		Clip *poNodeCompare;
		static ClipMan *posInstance;
		static CompareStrategyBase *posEnumNameCompare;

	};
}

#endif

//--- End of File ---
