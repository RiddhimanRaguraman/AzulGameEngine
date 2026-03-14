//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SkelMan.h"
#include "DLinkMan.h"
#include "SkelProto.h"
#include "SkelCompareStrategyEnumName.h"

namespace Azul
{

	SkelMan *SkelMan::posInstance = nullptr;
	CompareStrategyBase *SkelMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    SkelMan::SkelMan(int reserveNum, int reserveGrow)
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
    {
        // Preload the reserve
        this->proFillReservedPool(reserveNum);

        // initialize derived data here
        this->poNodeCompare = new Skel();
    }

    SkelMan::~SkelMan()
    {
        delete this->poNodeCompare;
        this->poNodeCompare = nullptr;

        // iterate through the list and delete
        Iterator *pIt = this->baseGetActiveIterator();

        DLink *pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Skel *pDeleteMe = (Skel *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }

        pIt = this->baseGetReserveIterator();

        pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Skel *pDeleteMe = (Skel *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }
    }

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
    void SkelMan::Create(int reserveNum, int reserveGrow)
    {
        // make sure values are ressonable 
        assert(reserveNum >= 0);
        assert(reserveGrow > 0);

        // initialize the singleton here
        assert(posInstance == nullptr);

        // Do the initialization
        if(posInstance == nullptr)
        {
            posInstance = new SkelMan(reserveNum, reserveGrow);
        }

        if (posEnumNameCompare == nullptr)
        {
            posEnumNameCompare = new SkelCompareStrategyEnumName();
        }

    }

    void SkelMan::Destroy()
    {
        SkelMan *pMan = SkelMan::privGetInstance();
        assert(pMan != nullptr);
        AZUL_UNUSED_VAR(pMan);

        delete SkelMan::posEnumNameCompare;
        SkelMan::posEnumNameCompare = nullptr;

        delete SkelMan::posInstance;
        SkelMan::posInstance = nullptr;
    }

    void SkelMan::Add(Skel::Name skelName, const char *const pFileName)
    {
        SkelProto(pFileName, skelName);
    }

    void SkelMan::Add(Skel::Name skelName, unsigned int numNodes, SkelEntry *poSkelEntry)
    {
        SkelMan *pMan = SkelMan::privGetInstance();

        Skel *pNode = (Skel *)pMan->baseAddToFront();
        assert(pNode != nullptr);

        pNode->Set(skelName, numNodes, poSkelEntry);
    }

    Skel *SkelMan::Find(Skel::Name _name)
    {
        SkelMan *pMan = SkelMan::privGetInstance();
        assert(pMan != nullptr);

        // set strategy
        pMan->pCompareStrategy = SkelMan::posEnumNameCompare;
        assert(pMan->pCompareStrategy);

        // Compare functions only compares two Nodes

        // So:  Use the Compare Node - as a reference
        //      use in the Compare() function
        pMan->poNodeCompare->mName = _name;

        Skel *pData = (Skel *)pMan->baseFind(pMan->poNodeCompare);
        return pData;
    }

    void SkelMan::Remove(Skel *pNode)
    {
        assert(pNode != nullptr);

        SkelMan *pMan = SkelMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseRemove(pNode);
    }

    void SkelMan::Dump()
    {
        SkelMan *pMan = SkelMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseDump();
    }

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
    SkelMan *SkelMan::privGetInstance()
    {
        // Safety - this forces users to call Create() first before using class
        assert(posInstance != nullptr);

        return posInstance;
    }

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
    DLink *SkelMan::derivedCreateNode()
    {
        DLink *pNodeBase = new Skel();
        assert(pNodeBase != nullptr);

        return pNodeBase;
    }
}

// --- End of File ---
