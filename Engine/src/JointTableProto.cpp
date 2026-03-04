//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "JointTableProto.h"
#include "File.h"
#include "JointTableMan.h"
#include "JointData.h"

namespace Azul
{
	JointTableProto::~JointTableProto()
	{
	}

	JointTableProto::JointTableProto(const char *const pFileName)
	{
		assert(pFileName);

		unsigned char *poBuff;
		unsigned int numBytes;

		File::Error ferror;
		ferror = File::GetFileAsBuffer(pFileName, poBuff, numBytes);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		// deserialize the data --> mB
		std::string strIn((const char *)poBuff, numBytes);
		JointData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		JointData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		this->numBones = mB.numBones;

		this->poJointTable = new unsigned int[this->numBones]();
		assert(this->poJointTable);
		size_t size = this->numBones * sizeof(unsigned int);
		memcpy(this->poJointTable, &mB.poJointEntry[0], size);

	}
}

//--- End of File ---
