//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ProtoBuf.h"

#include "GLTF.h"
#include "ConvertMesh.h"
#include "CreateTexture.h"
#include "CubeMesh.h"
#include "PyramidMesh.h"
#include "SphereMesh.h"
#include "SpriteMesh.h"
#include "ConvertXML.h"
#include "ConvertAnim.h"
#include "ConvertSkeleton.h"
#include "ConvertJoint.h"
#include "ConvertHierarchy.h"
#include "ConvertSkin.h"

#include <exception>
#include <future>
#include <utility>
#include <vector>

int main()
{
	// RAII - initialization
	ProtoBuf protobuf;

	// Copy clean copy of assets to Data
	system("copy .\\..\\Assets\\*.png .\\..\\Data");
	system("copy .\\..\\Assets\\*.tga .\\..\\Data");
	system("copy .\\..\\Assets\\*.glb .\\..\\Data");
	system("copy .\\..\\Assets\\*.xml .\\..\\Data");

	std::vector<std::future<void>> tasks;
	tasks.reserve(32);
	std::exception_ptr firstException = nullptr;

	auto addWork = [&](auto&& task)
		{
			tasks.emplace_back(std::async(std::launch::async, std::forward<decltype(task)>(task)));
		};

	addWork([]() { Azul::CreateTexturePNG("TEST_PNG_RGB.png", "TEST_PNG_RGB"); });
	addWork([]() { Azul::CreateTexturePNG("TEST_PNG_RGBA.png", "TEST_PNG_RGBA"); });
	addWork([]() { Azul::CreateTextureTGA("TEST_TGA_BGR.tga", "TEST_TGA_BGR"); });
	addWork([]() { Azul::CreateTextureTGA("TEST_TGA_BGRA.tga", "TEST_TGA_BGRA"); });

	addWork([]() { Azul::CreateTexturePNG("ChickenbotDiffuse.png", "ChickenBot"); });

	addWork([]() { Azul::CreateCubeMesh(); });
	addWork([]() { Azul::CreateSpriteMesh(); });

	addWork([]() { Azul::ConvertMesh("R2-D2.glb", "R2D2"); });
	addWork([]() { Azul::CreateTexturePNG("R2D2_BaseColor.png", "R2D2_Base"); });
	addWork([]() { Azul::CreateTexturePNG("R2D2_ColorID.png", "R2D2_Color"); });

	addWork([]() { Azul::ConvertMesh("space_frigate.glb", "space_frigate"); });
	addWork([]() { Azul::CreateTextureTGA("space_frigate.tga", "space_frigate"); });

	addWork([]() { Azul::ConvertMesh("woodencrate.glb", "crate"); });
	addWork([]() { Azul::CreateTexturePNG("crate_BaseColor.png", "crate"); });

	addWork([]() { Azul::CreateTexturePNG("FontArial36.png", "FontArial36"); });
	addWork([]() { Azul::ConvertXML("MetricsArial36.xml", "MetricsArial36"); });

	addWork([]() { Azul::ConvertSkin("ChickenBot_Mesh.glb", "Chickenbot"); });
	addWork([]() { Azul::ConvertJoint("ChickenBot_Mesh.glb", "ChickenBot"); });
	addWork([]() { Azul::ConvertHierarchy("ChickenBot_Mesh.glb", "ChickenBot"); });
	addWork([]() { Azul::ConvertSkeleton("walk_mesh.glb", "ChickenBot"); });
	addWork([]() { Azul::ConvertAnim("walk_mesh.glb", "ChickenBot_Walk"); });

	addWork([]() { Azul::ConvertJoint("Mousey_Mesh.glb", "Mousey"); });
	addWork([]() { Azul::ConvertSkin("Mousey_Mesh.glb", "Mousey"); });
	addWork([]() { Azul::ConvertSkeleton("Mousey_Mesh.glb", "Mousey"); });
	addWork([]() { Azul::ConvertHierarchy("Mousey_Mesh.glb", "Mousey"); });
	addWork([]() { Azul::ConvertAnim("Mousey_Anim_Silly_Dancing.glb", "Mousey_SillyDancing"); });
	addWork([]() { Azul::CreateTexturePNG("Mousey.png", "Mousey"); });
	addWork([]() { Azul::ConvertAnim("Mousey_Anim_Run_Forward.glb", "Mousey_Run"); });
	addWork([]() { Azul::ConvertAnim("Mousey_Anim_Gangnam_Style.glb", "Mousey_Gangnam"); });

	for (std::future<void>& f : tasks)
	{
		try
		{
			f.get();
		}
		catch (...)
		{
			if (firstException == nullptr)
			{
				firstException = std::current_exception();
			}
		}
	}

	if (firstException != nullptr)
	{
		try
		{
			std::rethrow_exception(firstException);
		}
		catch (const std::exception& e)
		{
			Trace::out("Converter worker threw exception: %s\n", e.what());
		}
		catch (...)
		{
			Trace::out("Converter worker threw unknown exception\n");
		}
		return 1;
	}


	//Azul::ConvertSkeleton("DogBot_Mesh.glb", "DogBot");
	//Azul::ConvertAnim("DogBot_Anim_Run.glb", "DogBot_Run");

	//Azul::ConvertSkeleton("SpiderBot_Mesh.glb", "SpiderBot");
	//Azul::ConvertAnim("SpiderBot_Anim_Walk.glb", "SpiderBot_Walk");

	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");
	system("del *.xml");
	system("del *.json");

	Trace::out("\n");
	return 0;

}

// ---  End of File ---
