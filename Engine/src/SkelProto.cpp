//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SkelProto.h"
#include "File.h"
#include "SkelMan.h"
#include "SkelData.h"

namespace Azul
{
	SkelProto::~SkelProto()
	{
	}

	SkelProto::SkelProto(const char *const pFileName, Skel::Name skelName)
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
		SkelData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		SkelData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		// Do I know this format?
		//assert(strcmp(mB.pVersion, "2.0.0") == 0);

		SkelMan::Add(skelName, mB.numNodes, mB.poSkelEntry);
	}
}

//--- End of File ---
