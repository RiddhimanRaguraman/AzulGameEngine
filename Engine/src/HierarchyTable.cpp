//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HierarchyTable.h"
#include "StringThis.h"

namespace Azul
{
	HierarchyTable::HierarchyTable()
		: DLink(),
		mName(HierarchyTable::Name::Not_Initialized),
		mNumBones(0),
		mDepth(0),
		poHierarchyTable(nullptr)
	{

	}

	HierarchyTable::~HierarchyTable()
	{
		delete[] this->poHierarchyTable;
	}

	size_t HierarchyTable::GetNumBones()
	{
		return this->mNumBones;
	}
	size_t HierarchyTable::GetDepth()
	{
		return this->mDepth;
	}
	void HierarchyTable::SetHierarchyTableName(Name HierarchyTableName)
	{
		this->mName = HierarchyTableName;
	}

	void HierarchyTable::Set(Name JointTableName, 
							 size_t _numBones, 
							 size_t _depth,
							 unsigned int *_pHierarchyTable)
	{
		this->mName = JointTableName;
		this->mNumBones = _numBones;
		this->mDepth = _depth;

		delete[] this->poHierarchyTable;

		assert(_pHierarchyTable);
		size_t count = this->mNumBones * this->mDepth;
		this->poHierarchyTable = new unsigned int[count]();
		size_t size = count* sizeof(unsigned int);
		memcpy(this->poHierarchyTable, _pHierarchyTable, size);
	}

	void HierarchyTable::Dump()
	{
		Trace::out("      HierarchyTable(%p)\n", this);
		Trace::out("      Name: %s \n", StringMe(this->mName));

		DLink::Dump();
	}

	void HierarchyTable::Wash()
	{
		this->mName = HierarchyTable::Name::Not_Initialized;
		this->mNumBones = 0;
		this->mDepth = 0;
		this->poHierarchyTable = nullptr;
	}

}

// --- End of File ---
