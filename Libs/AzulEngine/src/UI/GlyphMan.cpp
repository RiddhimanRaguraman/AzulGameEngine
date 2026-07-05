//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GlyphMan.h"
#include "DLinkMan.h"
#include "TexNodeMan.h"
#include "FontData.h"
#include "GlyphProto.h"
#include "GlyphCompareStrategyEnumName.h"

namespace Azul
{

	GlyphMan *GlyphMan::posInstance = nullptr;
	CompareStrategyBase *GlyphMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    GlyphMan::GlyphMan(int reserveNum, int reserveGrow)
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
    {
        // Preload the reserve
        this->proFillReservedPool(reserveNum);

        // initialize derived data here
        this->poNodeCompare = new Glyph();
    }

    GlyphMan::~GlyphMan()
    {
        delete this->poNodeCompare;
        this->poNodeCompare = nullptr;

        // iterate through the list and delete
        Iterator *pIt = this->baseGetActiveIterator();

        DLink *pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Glyph *pDeleteMe = (Glyph *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }

        pIt = this->baseGetReserveIterator();

        pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Glyph *pDeleteMe = (Glyph *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }
    }

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
    void GlyphMan::Create(int reserveNum, int reserveGrow)
    {
        // make sure values are ressonable 
        assert(reserveNum >= 0);
        assert(reserveGrow > 0);

        // initialize the singleton here
        assert(posInstance == nullptr);

        // Do the initialization
        if(posInstance == nullptr)
        {
            posInstance = new GlyphMan(reserveNum, reserveGrow);
        }

        if(posEnumNameCompare == nullptr)
        {
            posEnumNameCompare = new GlyphCompareStrategyEnumName();
        }

    }

    void GlyphMan::Destroy()
    {
        GlyphMan *pMan = GlyphMan::privGetInstance();
        assert(pMan != nullptr);
        AZUL_UNUSED_VAR(pMan);

        delete GlyphMan::posEnumNameCompare;
        GlyphMan::posEnumNameCompare = nullptr;

        delete GlyphMan::posInstance;
        GlyphMan::posInstance = nullptr;
    }

    Glyph *GlyphMan::Add(Glyph::Name glyphName, int key, TextureObject::Name texName, Rect &rect)
    {
        GlyphMan *pMan = GlyphMan::privGetInstance();

        Glyph *pNode = (Glyph *)pMan->baseAddToFront();
        assert(pNode != nullptr);

        // Initialize the date
        pNode->Set(glyphName, key, texName, rect);

        assert(pNode);
        return pNode;
    }

    void GlyphMan::Add(TextureObject::Name texName, const char *const pMetricsFileName)
    {
        GlyphProto(pMetricsFileName, texName);

    }

    Glyph *GlyphMan::Find(Glyph::Name _name, int _key)
    {
        GlyphMan *pMan = GlyphMan::privGetInstance();
        assert(pMan != nullptr);

        // set strategy
        pMan->pCompareStrategy = GlyphMan::posEnumNameCompare;
        assert(pMan->pCompareStrategy);

        // Compare functions only compares two Nodes

        // So:  Use the Compare Node - as a reference
        //      use in the Compare() function
        pMan->poNodeCompare->key = _key;
        pMan->poNodeCompare->glyphName = _name;

        Glyph *pData = (Glyph *)pMan->baseFind(pMan->poNodeCompare);
        return pData;
    }


    void GlyphMan::Remove(Glyph *pNode)
    {
        assert(pNode != nullptr);

        GlyphMan *pMan = GlyphMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseRemove(pNode);
    }

    void GlyphMan::Dump()
    {
        GlyphMan *pMan = GlyphMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseDump();
    }

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
    GlyphMan *GlyphMan::privGetInstance()
    {
        // Safety - this forces users to call Create() first before using class
        assert(posInstance != nullptr);

        return posInstance;
    }

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------
    DLink *GlyphMan::derivedCreateNode()
    {
        DLink *pNodeBase = new Glyph();
        assert(pNodeBase != nullptr);

        return pNodeBase;
    }
}

// --- End of File ---
