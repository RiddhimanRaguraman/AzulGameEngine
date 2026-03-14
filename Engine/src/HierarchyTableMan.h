//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef HIERARCHY_TABLE_MANAGER_H
#define HIERARCHY_TABLE_MANAGER_H

#include "ManBase.h"
#include "HierarchyTable.h"
#include "HierarchyData.h"

namespace Azul
{
	class HierarchyTableMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static HierarchyTable *Find(HierarchyTable::Name name);

		static void Remove(HierarchyTable *pNode);
		static void Dump();

		static HierarchyTable *Add(HierarchyTable::Name HierarchyTableName, const char *pFileName);

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static HierarchyTableMan *privGetInstance();

		HierarchyTableMan() = delete;
		HierarchyTableMan(const HierarchyTableMan &) = delete;
		HierarchyTableMan &operator=(const HierarchyTableMan &) = delete;
		virtual ~HierarchyTableMan();

		HierarchyTableMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		HierarchyTable *poNodeCompare;
		static HierarchyTableMan *posInstance;
		static CompareStrategyBase *posEnumNameCompare;
	};
}

#endif

//--- End of File ---
