//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "JointTable.h"
#include "StringThis.h"

namespace Azul
{
	JointTable::JointTable()
		: DLink(),
		mName(JointTable::Name::Not_Initialized),
		numBones(0),
		poJointTable(nullptr)
	{

	}

	JointTable::~JointTable()
	{
		delete[] this->poJointTable;
	}

	size_t JointTable::GetNumBones()
	{
		return this->numBones;
	}

	void JointTable::SetJointTableName(Name JointTableName)
	{
		this->mName = JointTableName;
	}

	void JointTable::Set(Name JointTableName, size_t _numBones, unsigned int *_pJointTable)
	{
		this->mName = JointTableName;
		this->numBones = _numBones;

		delete[] this->poJointTable;

		assert(_pJointTable);
		this->poJointTable = new unsigned int[this->numBones]();
		size_t size = this->numBones * sizeof(unsigned int);
		memcpy(this->poJointTable, _pJointTable, size);
	}

	void JointTable::Dump()
	{
		Trace::out("      JointTable(%p)\n", this);
		Trace::out("      Name: %s \n", StringMe(this->mName));

		DLink::Dump();
	}

	void JointTable::Wash()
	{
		this->mName = JointTable::Name::Not_Initialized;
		this->numBones = 0;
		this->poJointTable = nullptr;
	}

}

// --- End of File ---
