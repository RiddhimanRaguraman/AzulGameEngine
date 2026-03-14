//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "HierarchyData.h" 

namespace Azul
{
	HierarchyData::~HierarchyData()
	{
		delete[] this->poData;
		this->poData = nullptr;
	}

	HierarchyData::HierarchyData()
		:pVersion{0}, 
		numNodes(0),
		maxDepth(0),
		poData(nullptr)
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
	}

	HierarchyData::HierarchyData(unsigned int _numNodes, unsigned int _maxDepth)
		:pVersion{0}, 
		numNodes(_numNodes),
		maxDepth(_maxDepth),
		poData(nullptr)
	{
		strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
		assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);

		this->poData = new unsigned int[numNodes*maxDepth]();
		assert(this->poData);
	}

	HierarchyData &HierarchyData::operator = (const HierarchyData &r)
	{
		if(this != &r)
		{
			// Nuke orginal
			this->numNodes = 0;
			this->maxDepth = 0;
			delete[] this->poData;

			// copy the new
			this->numNodes = r.numNodes;
			this->maxDepth = r.maxDepth;
			this->poData = new unsigned int[numNodes * maxDepth]();

			for(size_t i = 0; i < (numNodes * maxDepth); i++)
			{
				this->poData[i] = r.poData[i];
			}
		}

		return *this;
	}

	void HierarchyData::Serialize(HierarchyData_proto &out) const
	{
		AZUL_UNUSED_VAR(out);

		std::string sVersion((const char *)this->pVersion, protoVersion::VERSION_NUM_BYTES);
		out.set_pversion(sVersion);

		out.set_numnodes(this->numNodes);
		out.set_maxdepth(this->maxDepth);

		for(unsigned int i = 0; i < (numNodes* maxDepth); i++)
		{
			// add it to the message(package)
			out.add_podata(this->poData[i]);
		}
	}

	void HierarchyData::Deserialize(const HierarchyData_proto &in)
	{
		AZUL_UNUSED_VAR(in);

		// CHECK the Version
		memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
		assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

		// Safety
		delete[] this->poData;
		this->poData = nullptr;

		this->numNodes = in.numnodes();
		this->maxDepth = in.maxdepth();

		this->poData = new unsigned int[(numNodes * maxDepth)]();
		assert(this->poData);

		for(unsigned int i = 0; i < (numNodes * maxDepth); i++)
		{
			poData[i] = in.podata(i);
		}
	}

	void HierarchyData::Print(const char *const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("numNodes: %d\n",this->numNodes);
		Trace::out("maxDepth: %d\n",this->maxDepth);


		for(size_t i = 0; i < (numNodes ); i++)
		{
			Trace::out("%2d: ", i);
			for(size_t j = 0; j < maxDepth; j++)
			{
				Trace::out("%2d ",this->poData[i * maxDepth + j]);
			}
			Trace::out("\n");
		}

		Trace::out("\n");
	}
}

// --- End of File ---


