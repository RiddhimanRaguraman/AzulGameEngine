//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "JointTableMan.h"
#include "DLinkMan.h"
#include "JointTableProto.h"
#include "File.h"
#include "JointTableCompareStrategyEnumName.h"
	
namespace Azul
{

	JointTableMan *JointTableMan::posInstance = nullptr;
	CompareStrategyBase *JointTableMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	JointTableMan::JointTableMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		this->poNodeCompare = new JointTable();
	}

	JointTableMan::~JointTableMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			JointTable *pDeleteMe = (JointTable *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			JointTable *pDeleteMe = (JointTable *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void JointTableMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(JointTableMan::posInstance == nullptr);

		// Do the initialization
		if(JointTableMan::posInstance == nullptr)
		{
			JointTableMan::posInstance = new JointTableMan(reserveNum, reserveGrow);
		}
		if (JointTableMan::posEnumNameCompare == nullptr)
		{
			JointTableMan::posEnumNameCompare = new JointTableCompareStrategyEnumName();
		}
	}

	void JointTableMan::Destroy()
	{
		JointTableMan *pMan = JointTableMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete JointTableMan::posInstance;
		JointTableMan::posInstance = nullptr;

		delete JointTableMan::posEnumNameCompare;
		JointTableMan::posEnumNameCompare = nullptr;
	}

	JointTable *JointTableMan::Add(JointTable::Name JointTableName, const char *pFileName)
	{
		JointTableMan *pMan = JointTableMan::privGetInstance();
		assert(pMan);

		JointTable *pNode = (JointTable *)pMan->baseAddToFront();
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
		JointData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		JointData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		pNode->Set(JointTableName, mB.numJoints, &mB.poJointEntry[0]);

		return pNode;
	}


	JointTable *JointTableMan::Find(JointTable::Name _name)
	{
		JointTableMan *pMan = JointTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->pCompareStrategy = JointTableMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->SetJointTableName(_name);

		JointTable *pData = (JointTable *)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}


	void JointTableMan::Remove(JointTable *pNode)
	{
		assert(pNode != nullptr);

		JointTableMan *pMan = JointTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void JointTableMan::Dump()
	{
		JointTableMan *pMan = JointTableMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	JointTableMan *JointTableMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *JointTableMan::derivedCreateNode()
	{
		DLink *pNodeBase = new JointTable();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}

// --- End of File ---
