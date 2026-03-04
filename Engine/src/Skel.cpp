//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Skel.h"
#include "StringThis.h"

namespace Azul
{
	// public methods: -------------------------------------------------------------

	Skel::Skel()
	{
		this->mName = Skel::Name::Not_Initialized;
		this->numBones = 0;;
		this->poTableArray = nullptr;
	}

	Skel::~Skel()
	{
		delete[] this->poTableArray;
		this->poTableArray = nullptr;
	}

	void Skel::Set(Skel::Name skelName, unsigned int _numBones, SkelEntry *poSkelEntry)
	{
		this->mName = skelName;
		this->numBones = _numBones;

		this->poTableArray = new Data[numBones]();

		assert(SkelEntry::SKELETON_NAME_SIZE == Skel::SKELETON_NAME_SIZE);

		for(size_t i = 0; i < numBones; i++)
		{
			poTableArray[i].index = poSkelEntry[i].nodeIndex;
			poTableArray[i].parentIndex = poSkelEntry[i].parentIndex;
			memcpy_s(poTableArray[i].name,
					 Skel::SKELETON_NAME_SIZE,
					 poSkelEntry[i].pNodeName,
					 SkelEntry::SKELETON_NAME_SIZE);
		}

	}



	void Skel::Dump()
	{
		Trace::out("      Skel(%p)\n", this);
		Trace::out("      Name: %s \n", StringMe(this->mName));

		DLink::Dump();
	}

	void Skel::Wash()
	{
		this->mName = Skel::Name::Not_Initialized;
		this->numBones = 0;
		// just in case
		delete[] this->poTableArray;
		this->poTableArray = nullptr;
	}
}

//--- End of File ---
