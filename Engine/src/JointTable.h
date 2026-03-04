//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef JOINT_TABLE_H
#define JOINT_TABLE_H

#include "DLink.h"

namespace Azul
{
	class JointTable : public DLink
	{
	public:
		enum Name
		{
			ChickenBot,
			Mousey,
			Not_Initialized
		};

	public:
		JointTable();
		JointTable(const JointTable &) = delete;
		JointTable &operator = (const JointTable &) = delete;
		virtual ~JointTable();

		size_t	GetNumBones();
		void SetJointTableName(Name JointTableName);

		void Set(Name JointTableName, size_t numBones, unsigned int *pJointTable);


		virtual void Dump() override;
		virtual void Wash() override;


	public:
		Name         mName;
		size_t       numBones;
		unsigned int *poJointTable;
	};
}

#endif

//--- End of File ---
