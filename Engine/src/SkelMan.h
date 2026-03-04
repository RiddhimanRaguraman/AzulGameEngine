//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKEL_MANAGER_H
#define SKEL_MANAGER_H

#include "ManBase.h"
#include "Skel.h"
#include "SkelData.h"

namespace Azul
{
    class SkelMan : public ManBase
    {
        //----------------------------------------------------------------------
        // Static Methods
        //----------------------------------------------------------------------
    public:
        static void Create(int reserveNum = 0, int reserveGrow = 1);
        static void Destroy();

        static Skel *Find(Skel::Name name);

        static void Remove(Skel *pNode);
        static void Dump();

        static void Add(const char *const pFileName, Skel::Name skelName);
        static void Add(Skel::Name skelName, unsigned int numBones, SkelEntry *poSkelEntry);

        //----------------------------------------------------------------------
        // Private methods
        //----------------------------------------------------------------------
    private:
        static SkelMan *privGetInstance();

        SkelMan() = delete;
        SkelMan(const SkelMan &) = delete;
        SkelMan &operator=(const SkelMan &) = delete;
        virtual ~SkelMan();

        SkelMan(int reserveNum, int reserveGrow);

        //----------------------------------------------------------------------
        // Override Abstract methods
        //----------------------------------------------------------------------
    protected:
        DLink *derivedCreateNode() override;

        //----------------------------------------------------------------------
        // Data: unique data for this manager 
        //----------------------------------------------------------------------
    private:
        Skel *poNodeCompare;
        static SkelMan *posInstance;
        static CompareStrategyBase *posEnumNameCompare;

    };
}

#endif

//--- End of File ---
