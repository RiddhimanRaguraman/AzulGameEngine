//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "SystemMan.h"
#include "System.h"

namespace Azul
{
	SystemMan *SystemMan::posInstance = nullptr;

	SystemMan::SystemMan()
		: mCount(0)
	{
		for (unsigned int i = 0; i < MAX_SYSTEMS; i++)
		{
			poSystems[i] = nullptr;
		}
	}

	void SystemMan::Create()
	{
		assert(posInstance == nullptr);
		posInstance = new SystemMan();
	}

	void SystemMan::Destroy()
	{
		SystemMan *p = SystemMan::privInstance();
		for (unsigned int i = 0; i < p->mCount; i++)
		{
			delete p->poSystems[i];
			p->poSystems[i] = nullptr;
		}
		delete p;
		posInstance = nullptr;
	}

	void SystemMan::Add(System *pSystem)
	{
		assert(pSystem);
		SystemMan *p = SystemMan::privInstance();
		assert(p->mCount < MAX_SYSTEMS);
		p->poSystems[p->mCount] = pSystem;
		p->mCount++;
	}

	void SystemMan::Run(World &world, AnimTime tCurr)
	{
		SystemMan *p = SystemMan::privInstance();
		for (unsigned int i = 0; i < p->mCount; i++)
		{
			p->poSystems[i]->Update(world, tCurr);
		}
	}

	SystemMan *SystemMan::privInstance()
	{
		assert(posInstance);
		return posInstance;
	}
}

// --- End of File ---
