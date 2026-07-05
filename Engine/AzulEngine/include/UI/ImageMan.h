//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "ManBase.h"
#include "Image.h"

namespace Azul
{
	class ImageMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();
		static Image *Add(Image::Name imageName, TextureObject::Name textName, Rect rect);

		static Image *Find(Image::Name name);

		static void Remove(Image *pNode);
		static void Dump();

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static ImageMan *privGetInstance();

		ImageMan() = delete;
		ImageMan(const ImageMan &) = delete;
		ImageMan &operator=(const ImageMan &) = delete;
		virtual ~ImageMan();

		ImageMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		Image *poNodeCompare;
		static ImageMan *posInstance;
		static CompareStrategyBase* posEnumNameCompare;

	};
}

#endif

//--- End of File ---
