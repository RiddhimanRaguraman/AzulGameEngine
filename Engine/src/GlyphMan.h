//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include "ManBase.h"
#include "Glyph.h"

namespace Azul
{
    class GlyphMan : public ManBase
    {
        //----------------------------------------------------------------------
        // Static Methods
        //----------------------------------------------------------------------
    public:
        static void Create(int reserveNum = 0, int reserveGrow = 1);
        static void Destroy();

        static Glyph *Add(Glyph::Name glyphName, int key, TextureObject::Name textName, Rect &rect);
        static void Add(TextureObject::Name texName, const char *const pMetricsFileName);


        static Glyph *Find(Glyph::Name _name, int _key);

        static void Remove(Glyph *pNode);
        static void Dump();

        //----------------------------------------------------------------------
        // Private methods
        //----------------------------------------------------------------------
    private:
        static GlyphMan *privGetInstance();

        GlyphMan() = delete;
        GlyphMan(const GlyphMan &) = delete;
        GlyphMan &operator=(const GlyphMan &) = delete;
        virtual ~GlyphMan();

        GlyphMan(int reserveNum, int reserveGrow);

        //----------------------------------------------------------------------
        // Override Abstract methods
        //----------------------------------------------------------------------
    protected:
        DLink *derivedCreateNode() override;

        //----------------------------------------------------------------------
        // Data: unique data for this manager 
        //----------------------------------------------------------------------
    private:
        Glyph *poNodeCompare;
        static GlyphMan *posInstance;
        static CompareStrategyBase *posEnumNameCompare;

    };
}

#endif

//--- End of File ---
