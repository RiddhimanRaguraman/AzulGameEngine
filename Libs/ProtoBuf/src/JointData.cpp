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
		this->numBones = 0;
		this->poJointEntry = nullptr;
	}

	JointData::JointData(unsigned int _numBones, unsigned int* pData)
	{
		this->numBones = _numBones;

		this->poJointEntry = new unsigned int[_numBones]();
		assert(this->poJointEntry);

		size_t size = this->numBones * sizeof(unsigned int);
		memcpy(this->poJointEntry, pData, size);
	}

	JointData& JointData::operator = (const JointData& r)
	{
		if (this != &r)
		{
			// Nuke orginal
			this->numBones = 0;
			delete[] this->poJointEntry;

			// copy the new
			this->numBones = r.numBones;
			this->poJointEntry = new unsigned int[r.numBones]();

			for (size_t i = 0; i < r.numBones; i++)
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
		out.set_numbones(this->numBones);

		// Add the SkelEntry
		JointData_proto tmpBoneEntry_proto;

		for (unsigned int i = 0; i < this->numBones; i++)
		{
			out.add_pojointentry(this->poJointEntry[i]);
		}

		assert(out.numbones() == this->numBones);
	}

	void JointData::Deserialize(const JointData_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		// Safety
		delete[] this->poJointEntry;
		this->poJointEntry = nullptr;

		this->numBones = in.numbones();

		this->poJointEntry = new unsigned int[this->numBones]();
		assert(this->poJointEntry);

		for (unsigned int i = 0; i < this->numBones; i++)
		{
			poJointEntry[i] = in.pojointentry(i);
		}
	}

	void JointData::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("     numBones: %d\n", this->numBones);

		for (size_t i = 0; i < this->numBones; i++)
		{
			Trace::out("      [%d]: jointIndex: %d\n", i, this->poJointEntry[i]);
		}

		Trace::out("\n");
	}
}

// --- End of File ---

