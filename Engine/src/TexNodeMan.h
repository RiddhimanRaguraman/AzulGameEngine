//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEX_NODE_MAN_H
#define TEX_NODE_MAN_H 

#include "ManBase.h"
#include "TexNode.h"
#include "TextureObject.h"

namespace Azul
{

	class TexNodeMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static TexNode *Add(const char *pFile, TextureObject::Name name);
		static TexNode *Add(TextureObject::Name name, TextureObject *pTextureObject);
		static TextureObject *Find(TextureObject::Name name);

		static void Remove(TexNode *pNode);
		static void Dump();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static TexNodeMan *privGetInstance();

		TexNodeMan() = delete;
		TexNodeMan(const TexNodeMan &) = delete;
		TexNodeMan &operator = (const TexNodeMan &) = delete;
		virtual ~TexNodeMan();

		TexNodeMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		TexNode *poNodeCompare;
		static TexNodeMan *posInstance;
		static CompareStrategyBase *posEnumNameCompare;

	};

}

#endif

// --- End of File ---
