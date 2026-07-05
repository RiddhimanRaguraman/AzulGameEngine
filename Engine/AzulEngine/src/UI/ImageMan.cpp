//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ImageMan.h"
#include "DLinkMan.h"
#include "TexNodeMan.h"
#include "ImageCompareStrategyEnumName.h"

namespace Azul
{

	ImageMan* ImageMan::posInstance = nullptr;
	CompareStrategyBase* ImageMan::posEnumNameCompare = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	ImageMan::ImageMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		this->poNodeCompare = new Image();
	}

	ImageMan::~ImageMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator* pIt = this->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			Image* pDeleteMe = (Image*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			Image* pDeleteMe = (Image*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void ImageMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new ImageMan(reserveNum, reserveGrow);
		}

		if (posEnumNameCompare == nullptr)
		{
			posEnumNameCompare = new ImageCompareStrategyEnumName();
		}
	}

	void ImageMan::Destroy()
	{
		ImageMan* pMan = ImageMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete ImageMan::posEnumNameCompare;
		ImageMan::posEnumNameCompare = nullptr;

		delete ImageMan::posInstance;
		ImageMan::posInstance = nullptr;
	}

	Image* ImageMan::Add(Image::Name imageName, TextureObject::Name textName, Rect rect)

	{
		ImageMan* pMan = ImageMan::privGetInstance();

		Image* pNode = (Image*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		TextureObject* pText = TexNodeMan::Find(textName);
		assert(pText);

		// Initialize the date
		pNode->Set(imageName, pText, rect);

		assert(pNode);
		return pNode;
	}


	Image* ImageMan::Find(Image::Name _name)
	{
		ImageMan* pMan = ImageMan::privGetInstance();
		assert(pMan != nullptr);

		// set strategy
		pMan->pCompareStrategy = ImageMan::posEnumNameCompare;
		assert(pMan->pCompareStrategy);

		pMan->poNodeCompare->imageName = _name;

		Image* pData = (Image*)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}


	void ImageMan::Remove(Image* pNode)
	{
		assert(pNode != nullptr);

		ImageMan* pMan = ImageMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void ImageMan::Dump()
	{
		ImageMan* pMan = ImageMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	ImageMan* ImageMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink* ImageMan::derivedCreateNode()
	{
		DLink* pNodeBase = new Image();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}

// --- End of File ---
