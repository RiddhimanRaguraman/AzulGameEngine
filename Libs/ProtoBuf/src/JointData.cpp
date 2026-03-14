//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "JointData.h"

namespace Azul
{
	JointData::~JointData()
	{
		delete[] this->poJointEntry;
		this->poJointEntry = nullptr;
	}

	JointData::JointData()
	{
		this->numJoints = 0;
		this->poJointEntry = nullptr;
	}

	JointData::JointData(unsigned int _numJoints, unsigned int* pData)
	{
		this->numJoints = _numJoints;

		this->poJointEntry = new unsigned int[_numJoints]();
		assert(this->poJointEntry);

		size_t size = this->numJoints * sizeof(unsigned int);
		memcpy(this->poJointEntry, pData, size);
	}

	JointData& JointData::operator = (const JointData& r)
	{
		if (this != &r)
		{
			// Nuke orginal
			this->numJoints = 0;
			delete[] this->poJointEntry;

			// copy the new
			this->numJoints = r.numJoints;
			this->poJointEntry = new unsigned int[r.numJoints]();

			for (size_t i = 0; i < r.numJoints; i++)
			{
				this->poJointEntry[i] = r.poJointEntry[i];
			}
		}

		return *this;
	}

	void JointData::Serialize(JointData_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		// Add the number of bones
		out.set_numjoints(this->numJoints);

		// Add the SkelEntry
		JointData_proto tmpBoneEntry_proto;

		for (unsigned int i = 0; i < this->numJoints; i++)
		{
			out.add_pojointentry(this->poJointEntry[i]);
		}

		assert(out.numjoints() == this->numJoints);
	}

	void JointData::Deserialize(const JointData_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		// Safety
		delete[] this->poJointEntry;
		this->poJointEntry = nullptr;

		this->numJoints = in.numjoints();

		this->poJointEntry = new unsigned int[this->numJoints]();
		assert(this->poJointEntry);

		for (unsigned int i = 0; i < this->numJoints; i++)
		{
			poJointEntry[i] = in.pojointentry(i);
		}
	}

	void JointData::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("     numJoints: %d\n", this->numJoints);

		for (size_t i = 0; i < this->numJoints; i++)
		{
			Trace::out("      [%d]: jointIndex: %d\n", i, this->poJointEntry[i]);
		}

		Trace::out("\n");
	}
}

// --- End of File ---

