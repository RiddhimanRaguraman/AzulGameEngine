//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "SkelData.h"

namespace Azul
{
	SkelData::~SkelData()
	{
		delete[] this->poSkelEntry;
		this->poSkelEntry = nullptr;
	}

	SkelData::SkelData()
		: pVersion{ 0 },
		numNodes{ 0 },
		poSkelEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
	}

	SkelData::SkelData(unsigned int _numBones)
		: pVersion{ 0 },
		numNodes{ _numBones },
		poSkelEntry{ nullptr }
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);

		this->poSkelEntry = new SkelEntry[_numBones]();
		assert(this->poSkelEntry);
	}

	void SkelData::Serialize(SkelData_proto& out) const
	{
		AZUL_UNUSED_VAR(out);

		std::string sVersion((const char*)this->pVersion, protoVersion::VERSION_NUM_BYTES);
		out.set_pversion(sVersion);

		// Add the number of bones
		out.set_numnodes(this->numNodes);

		// Add the SkelEntry
		SkelEntry_proto tmpSkelEntry_proto;

		for (unsigned int i = 0; i < this->numNodes; i++)
		{
			// create one proto
			this->poSkelEntry[i].Serialize(tmpSkelEntry_proto);

			// add it to the message(package)
			SkelEntry_proto* p = out.add_poskelentry();
			assert(p);
			*p = tmpSkelEntry_proto;
		}

		assert(out.numnodes() == this->numNodes);
	}

	void SkelData::Deserialize(const SkelData_proto& in)
	{
		AZUL_UNUSED_VAR(in);
		
		// CHECK the Version
		memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
		assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

		// safety
		delete[] this->poSkelEntry;
		this->poSkelEntry = nullptr;

		this->numNodes = in.numnodes();

		this->poSkelEntry = new SkelEntry[this->numNodes]();
		assert(this->poSkelEntry);

		for (unsigned int i = 0; i < this->numNodes; i++)
		{
			poSkelEntry[i].Deserialize(in.poskelentry((int)i));
		}
	}

	void SkelData::Print(const char* const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		for (size_t i = 0; i < this->numNodes; i++)
		{
			this->poSkelEntry[i].Print(pName);
		}

		Trace::out("\n");


	}
}

// --- End of File ---


