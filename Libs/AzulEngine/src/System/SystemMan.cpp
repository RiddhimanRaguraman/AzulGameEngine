#include "SystemMan.h"
#include "System.h"
#include "WorldMan.h"
#include "World.h"
#include "AnimTimer.h"
#include "Profiler.h"

#include "LocalToWorldSystem.h"
#include "RotateSystem.h"
#include "AnimationSystem.h"
#include "BlendSystem.h"
#include "SkinningSystem.h"
#include "RenderSystem.h"
#include "SpriteRenderSystem.h"

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

		// The standard engine systems, in run order (was GameObjectMan::Create).
		// LocalToWorld composes world = S*R*T; RotateSystem spins RotateComponent
		// entities (overwrites the plain world, so after LocalToWorld);
		// Animation/Blend sample clips into the mixer buffers; Skinning then
		// dispatches the GPU compute skinning that consumes them. Each is a no-op
		// in scenes without the matching components.
		SystemMan::Add(new LocalToWorldSystem());
		SystemMan::Add(new RotateSystem());
		SystemMan::Add(new AnimationSystem());
		SystemMan::Add(new BlendSystem());
		SystemMan::Add(new SkinningSystem());
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

	void SystemMan::Run(World &world, AnimTime tDelta)
	{
		AnimTimer timer;
		timer.Tic();

		SystemMan *p = SystemMan::privInstance();
		for (unsigned int i = 0; i < p->mCount; i++)
		{
			p->poSystems[i]->Update(world, tDelta);
		}

		Profiler::AddUpdate(timer.Toc());
	}

	void SystemMan::Draw(World &world)
	{
		AnimTimer timer;
		timer.Tic();

		// Pool-driven draw (was GameObjectMan::Draw): the layer-sorted 3D pass
		// then the 2D/UI pass (sprites + text), after the 3D pass.
		RenderSystem::Draw(world);
		SpriteRenderSystem::Draw(world);

		Profiler::AddDraw(timer.Toc());
		// Draw is called once per frame -> close the profiler frame here.
		Profiler::EndFrame();
	}

	SystemMan *SystemMan::privInstance()
	{
		assert(posInstance);
		return posInstance;
	}
}

// --- End of File ---
