//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshNodeMan.h"
#include "MeshNull.h"
#include "ManBase.h"
#include "DLinkMan.h"
#include "MeshNodeCompareStrategyCharName.h"
#include "MeshNodeCompareStrategyEnumName.h"
#include "MeshProto.h"

namespace Azul
{
	MeshNodeMan *MeshNodeMan::posInstance = nullptr;
	CompareStrategyBase *MeshNodeMan::posCharNameCompare = nullptr;
	CompareStrategyBase *MeshNodeMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	MeshNodeMan::MeshNodeMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// -----------------------
		// Compare Node create
		// -----------------------
		Mesh *pMesh = new MeshNull();
		assert(pMesh);
		this->poNodeCompare = new MeshNode();
		assert(this->poNodeCompare);
		this->poNodeCompare->Set(Mesh::Name::NULL_MESH, pMesh);
	}

	MeshNodeMan::~MeshNodeMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			MeshNode *pDeleteMe = (MeshNode *) pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			MeshNode *pDeleteMe = (MeshNode *) pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void MeshNodeMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if(posInstance == nullptr)
		{
			posInstance = new MeshNodeMan(reserveNum, reserveGrow);
		}
		if(posEnumNameCompare == nullptr)
		{
			posEnumNameCompare = new MeshNodeCompareStrategyEnumName();
		}
		if(posCharNameCompare == nullptr)
		{
			posCharNameCompare = new MeshNodeCompareStrategyCharName();
		}
	}

	void MeshNodeMan::Destroy()
	{
		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete MeshNodeMan::posEnumNameCompare;
		MeshNodeMan::posEnumNameCompare = nullptr;

		delete MeshNodeMan::posCharNameCompare;
		MeshNodeMan::posCharNameCompare = nullptr;

		delete MeshNodeMan::posInstance;
		MeshNodeMan::posInstance = nullptr;
	}

	MeshNode *MeshNodeMan::Add(Mesh::Name name, const char *pFile)
	{
		assert(pFile);

		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan);

		Mesh *pMesh = new MeshProto(pFile);
		assert(pMesh);

		MeshNode *pNode = (MeshNode *)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, pMesh);

		return pNode;
	}

	MeshNode *MeshNodeMan::Add(Mesh::Name name, Mesh *pMesh)
	{
		assert(pMesh);

		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan);

		MeshNode *pNode = (MeshNode *)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, pMesh);

		return pNode;
	}

	Mesh *MeshNodeMan::Find(Mesh::Name _name)
	{
		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = MeshNodeMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		Mesh *pMesh = pMan->poNodeCompare->GetMesh();
		assert(pMesh);

		pMesh->name = _name;

		MeshNode *pData = (MeshNode *) pMan->baseFind(pMan->poNodeCompare);

    if(pData)
    {
        pMesh = pData->GetMesh();
    }
    else
    {
        pMesh = nullptr;
    }

    return pMesh;
	}

	Mesh *MeshNodeMan::Find(const char *pModelName)
	{
		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = MeshNodeMan::posCharNameCompare;
		assert(pMan->pCompareStrategy);

		Mesh *pMesh = pMan->poNodeCompare->GetMesh();
		assert(pMesh);

		strcpy_s(pMesh->pModelName, Mesh::MESH_MODEL_NAME_SIZE, pModelName);

		MeshNode *pData = (MeshNode *)pMan->baseFind(pMan->poNodeCompare);

		if (pData)
		{
			pMesh = pData->GetMesh();
		}
		else
		{
			pMesh = nullptr;
		}

		return pMesh;
	}

	void MeshNodeMan::Remove(MeshNode *pNode)
	{
		assert(pNode != nullptr);

		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void MeshNodeMan::Dump()
	{
		MeshNodeMan *pMan = MeshNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	MeshNodeMan *MeshNodeMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *MeshNodeMan::derivedCreateNode()
	{
		DLink *pNodeBase = new MeshNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}

}

// --- End of File ---
