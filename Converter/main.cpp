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
#include "ConvertSkin.h"

int main()
{
	// RAII - initialization
	ProtoBuf protobuf;

	// Copy clean copy of assets to Data
	system("copy .\\..\\Assets\\*.png .\\..\\Data");
	system("copy .\\..\\Assets\\*.tga .\\..\\Data");
	system("copy .\\..\\Assets\\*.glb .\\..\\Data");
	system("copy .\\..\\Assets\\*.xml .\\..\\Data");

	// Textures
	Azul::CreateTexturePNG("TEST_PNG_RGB.png", "TEST_PNG_RGB");
	Azul::CreateTexturePNG("TEST_PNG_RGBA.png", "TEST_PNG_RGBA");
	Azul::CreateTextureTGA("TEST_TGA_BGR.tga", "TEST_TGA_BGR");
	Azul::CreateTextureTGA("TEST_TGA_BGRA.tga", "TEST_TGA_BGRA");

	Azul::CreateTexturePNG("ChickenbotDiffuse.png", "ChickenBot");
	//Azul::CreateTexturePNG("DogbotDiffuse.png", "DogBot");
	//Azul::CreateTexturePNG("Spiderbot_diffuse.png", "SpiderBot");
	
	//// Mesh
	Azul::CreateCubeMesh();
	Azul::CreateSpriteMesh();

	Azul::ConvertMesh("R2-D2.glb", "R2D2");
	Azul::CreateTexturePNG("R2D2_BaseColor.png", "R2D2_Base");
	Azul::CreateTexturePNG("R2D2_ColorID.png", "R2D2_Color");

	Azul::ConvertMesh("space_frigate.glb", "space_frigate");
	Azul::CreateTextureTGA("space_frigate.tga", "space_frigate");

	Azul::ConvertMesh("woodencrate.glb", "crate");
	Azul::CreateTexturePNG("crate_BaseColor.png", "crate");

	//// Font stuff
	Azul::CreateTexturePNG("FontArial36.png", "FontArial36");
	Azul::ConvertXML("MetricsArial36.xml", "MetricsArial36");

	//char* poJSON = Azul::GLTF::ExportJSON("walk_mesh.glb");

	//// Animation

	Azul::ConvertSkin("ChickenBot_Mesh.glb", "Chickenbot");
	Azul::ConvertJoint("ChickenBot_Mesh.glb", "ChickenBot");
	Azul::ConvertSkeleton("walk_mesh.glb", "ChickenBot");
	Azul::ConvertAnim("walk_mesh.glb", "ChickenBot_Walk");
	//Azul::ConvertAnim("runRM+mesh.glb", "ChickenBot_Run");
	//Azul::ConvertAnim("hit_back+mesh.glb", "ChickenBot_HitBack");
	//Azul::ConvertAnim("idle+mesh.glb", "ChickenBot_Idle");
	//Azul::ConvertAnim("shot_up+mesh.glb", "ChickenBot_ShotUp");

	//delete poJSON;

	// Mousey
	Azul::ConvertJoint("Mousey_Mesh.glb", "Mousey");
	Azul::ConvertSkin("Mousey_Mesh.glb", "Mousey");
	Azul::ConvertSkeleton("Mousey_Mesh.glb", "Mousey");
	Azul::ConvertAnim("Mousey_Anim_Silly_Dancing.glb", "Mousey_SillyDancing");
	Azul::CreateTexturePNG("Mousey.png", "Mousey");


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
