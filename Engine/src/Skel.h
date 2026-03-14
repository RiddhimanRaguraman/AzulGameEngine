//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SKEL_H
#define SKEL_H

#include "DLink.h"
#include "SkelData.h"

namespace Azul
{
	class Skel : public DLink
	{
	public:
		static const unsigned int SKELETON_NAME_SIZE = 64;

		enum Name
		{
			ChickenBot,
			DogBot,
			SpiderBot,
			Mousey,
			Not_Initialized
		};

		struct Data
		{
			int  index;
			int  parentIndex;
			char name[SKELETON_NAME_SIZE];
		};

	public:
		// public methods: -------------------------------------------------------------
		Skel();
		Skel(const Skel &) = delete;
		Skel &operator = (const Skel &) = delete;
		virtual ~Skel();



		virtual void Dump() override;
		virtual void Wash() override;

		void Set(Skel::Name skelName, unsigned int numNodes, SkelEntry *poSkelEntry);

	public:
		Name           mName;
		unsigned int   numNodes;
		Data          *poTableArray;

	};

}


#endif

//--- End of File ---
