//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ClipMan.h"
#include "DLinkMan.h"
#include "ClipProto.h"
#include "ClipCompareStrategyEnumName.h"

namespace Azul
{

	ClipMan *ClipMan::posInstance = nullptr;
	CompareStrategyBase *ClipMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	ClipMan::ClipMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		this->poNodeCompare = new Clip();
	}

	ClipMan::~ClipMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			Clip *pDeleteMe = (Clip *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			Clip *pDeleteMe = (Clip *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void ClipMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(ClipMan::posInstance == nullptr);

		// Do the initialization
		if(ClipMan::posInstance == nullptr)
		{
			ClipMan::posInstance = new ClipMan(reserveNum, reserveGrow);
		}

		if (ClipMan::posEnumNameCompare == nullptr)
		{
			ClipMan::posEnumNameCompare = new ClipCompareStrategyEnumName();
		}

	}

	void ClipMan::Destroy()
	{
		ClipMan *pMan = ClipMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete ClipMan::posEnumNameCompare;
		ClipMan::posEnumNameCompare = nullptr;

		delete ClipMan::posInstance;
		ClipMan::posInstance = nullptr;
	}

	void ClipMan::Add(const char *const pFileName, Clip::Name clipName, Skel::Name skelName)
	{
		ClipProto(pFileName, clipName, skelName);
	}

	void ClipMan::Add(Clip::Name clipName,
					  unsigned int numBones,
					  unsigned int numKeyFrames,
					  AnimFrameBucket *pFrameBucket,
					  Skel::Name skelName)
	{
		ClipMan *pMan = ClipMan::privGetInstance();

		Clip *pNode = (Clip *)pMan->baseAddToFront();
		assert(pNode != nullptr);

		pNode->Set(clipName, numBones, numKeyFrames, pFrameBucket, skelName);
	}

	Clip *ClipMan::Find(Clip::Name _name)
	{
		ClipMan *pMan = ClipMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = ClipMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		pMan->poNodeCompare->SetClipName(_name);

		Clip *pData = (Clip *)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}


	void ClipMan::Remove(Clip *pNode)
	{
		assert(pNode != nullptr);

		ClipMan *pMan = ClipMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void ClipMan::Dump()
	{
		ClipMan *pMan = ClipMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	ClipMan *ClipMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *ClipMan::derivedCreateNode()
	{
		DLink *pNodeBase = new Clip();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}

// --- End of File ---
