//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ClipProto.h"
#include "File.h"
#include "ClipMan.h"
#include "AnimClip.h"

namespace Azul
{
	ClipProto::~ClipProto()
	{
	}

	ClipProto::ClipProto(const char *const pFileName, Clip::Name clipName, Skel::Name skelName, HierarchyTable::Name hierarchyName)
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
		AnimClip_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		AnimClip mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		// Do I know this format?
		//assert(strcmp(mB.pVersion, "2.0.0") == 0);

		ClipMan::Add(clipName,
					 mB.numNodes,
					 mB.numKeyFrames,
					 mB.poFrameBucketEntry,
					 skelName,
					 hierarchyName);

	}
}

//--- End of File ---
