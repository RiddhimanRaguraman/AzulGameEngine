//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "HierarchyTableMan.h"
#include "DLinkMan.h"
#include "HierarchyTableProto.h"
#include "File.h"
#include "HierarchyTableCompareStrategyEnumName.h"

namespace Azul
{

	HierarchyTableMan *HierarchyTableMan::posInstance = nullptr;
	CompareStrategyBase *HierarchyTableMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	HierarchyTableMan::HierarchyTableMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		this->poNodeCompare = new HierarchyTable();
	}

	HierarchyTableMan::~HierarchyTableMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			HierarchyTable *pDeleteMe = (HierarchyTable *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			HierarchyTable *pDeleteMe = (HierarchyTable *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void HierarchyTableMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if(posInstance == nullptr)
		{
			posInstance = new HierarchyTableMan(reserveNum, reserveGrow);
		}

		if(posEnumNameCompare == nullptr)
		{
			posEnumNameCompare = new HierarchyTableCompareStrategyEnumName();
		}

	}

	void HierarchyTableMan::Destroy()
	{
		HierarchyTableMan *pMan = HierarchyTableMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete HierarchyTableMan::posInstance;
		HierarchyTableMan::posInstance = nullptr;

		delete posEnumNameCompare;
	}

	HierarchyTable *HierarchyTableMan::Add(HierarchyTable::Name JointTableName, const char *pFileName)
	{
		HierarchyTableMan *pMan = HierarchyTableMan::privGetInstance();
		assert(pMan);

		HierarchyTable *pNode = (HierarchyTable *)pMan->baseAddToFront();
		assert(pNode != nullptr);


		assert(pFileName);

		unsigned char *poBuff;
		unsigned int numBytes;

		File::Error ferror;
		ferror = File::GetFileAsBuffer(pFileName, poBuff, numBytes);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		// deserialize the data --> mB
		std::string strIn((const char *)poBuff, numBytes);
		HierarchyData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		HierarchyData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		pNode->Set(JointTableName, 
				   mB.numJoints, 
				   mB.maxDepth,
				   mB.poData);

		return pNode;
	}


	HierarchyTable *HierarchyTableMan::Find(HierarchyTable::Name _name)
	{
		HierarchyTableMan *pMan = HierarchyTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->pCompareStrategy = HierarchyTableMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->SetHierarchyTableName(_name);

		HierarchyTable *pData = (HierarchyTable *)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}

	void HierarchyTableMan::Remove(HierarchyTable *pNode)
	{
		assert(pNode != nullptr);

		HierarchyTableMan *pMan = HierarchyTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void HierarchyTableMan::Dump()
	{
		HierarchyTableMan *pMan = HierarchyTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	HierarchyTableMan *HierarchyTableMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *HierarchyTableMan::derivedCreateNode()
	{
		DLink *pNodeBase = new HierarchyTable();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}

// --- End of File ---
