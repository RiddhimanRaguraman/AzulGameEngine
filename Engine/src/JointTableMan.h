//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef JOINT_TABLE_MANAGER_H
#define JOINT_TABLE_MANAGER_H

#include "ManBase.h"
#include "JointTable.h"
#include "JointData.h"

namespace Azul
{
	class JointTableMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static JointTable *Find(JointTable::Name name);

		static void Remove(JointTable *pNode);
		static void Dump();

		static JointTable *Add(JointTable::Name JointTableName, const char *pFileName);

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static JointTableMan *privGetInstance();

		JointTableMan() = delete;
		JointTableMan(const JointTableMan &) = delete;
		JointTableMan &operator=(const JointTableMan &) = delete;
		virtual ~JointTableMan();

		JointTableMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		JointTable *poNodeCompare;
		static JointTableMan *posInstance;

	};
}

#endif

//--- End of File ---
