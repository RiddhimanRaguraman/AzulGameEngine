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

int main()
{
	// RAII - initialization
	ProtoBuf protobuf;

	// Copy clean copy of assets to Data
	system("copy .\\..\\Assets\\*.png .\\..\\Data");
	system("copy .\\..\\Assets\\*.tga .\\..\\Data");
	system("copy .\\..\\Assets\\*.glb .\\..\\Data");
	system("copy .\\..\\Assets\\*.xml .\\..\\Data");

	Azul::CreateCubeMesh();  
	Azul::CreateSpriteMesh();  

	Azul::CreateTexturePNG("Header.png", "Header");  
	Azul::ConvertXML("HeaderMetrics.xml", "HeaderMetrics");  

	Azul::ConvertSkin("Halo_Shuffling.glb", "Halo");  
	Azul::ConvertJoint("Halo_Shuffling.glb", "Halo");  
	Azul::ConvertHierarchy("Halo_Shuffling.glb", "Halo");  
	Azul::ConvertSkeleton("Halo_Shuffling.glb", "Halo");  
	Azul::CreateTexturePNG("Halo_Diffuse.png", "Halo");  
	Azul::ConvertAnim("Halo_Shuffling.glb", "Halo_Shuffling");  

	Azul::ConvertSkin("Crownboi_Rumba.glb", "Crownboi");  
	Azul::ConvertJoint("Crownboi_Rumba.glb", "Crownboi");  
	Azul::ConvertHierarchy("Crownboi_Rumba.glb", "Crownboi");  
	Azul::ConvertSkeleton("Crownboi_Rumba.glb", "Crownboi");  
	Azul::CreateTexturePNG("Crownboi_Diffuse.png", "Crownboi");  
	Azul::ConvertAnim("Crownboi_Rumba.glb", "Crownboi_Rumba");  

	Azul::ConvertSkin("Drax_Swing.glb", "Drax");  
	Azul::ConvertJoint("Drax_Swing.glb", "Drax");  
	Azul::ConvertHierarchy("Drax_Swing.glb", "Drax");  
	Azul::ConvertSkeleton("Drax_Swing.glb", "Drax");  
	Azul::CreateTexturePNG("Drax_Diffuse.png", "Drax");  
	Azul::ConvertAnim("Drax_Swing.glb", "Drax_Swing");  

	Azul::ConvertSkin("Maw_Breakdance.glb", "Maw");  
	Azul::ConvertJoint("Maw_Breakdance.glb", "Maw");  
	Azul::ConvertHierarchy("Maw_Breakdance.glb", "Maw");  
	Azul::ConvertSkeleton("Maw_Breakdance.glb", "Maw");  
	Azul::CreateTexturePNG("Maw_Diffuse.png", "Maw");  
	Azul::ConvertAnim("Maw_Breakdance.glb", "Maw_Breakdance");  

	Azul::ConvertSkin("Pirate_Salsa.glb", "Pirate");  
	Azul::ConvertJoint("Pirate_Salsa.glb", "Pirate");  
	Azul::ConvertHierarchy("Pirate_Salsa.glb", "Pirate");  
	Azul::ConvertSkeleton("Pirate_Salsa.glb", "Pirate");  
	Azul::CreateTexturePNG("Pirate_Diffuse.png", "Pirate");  
	Azul::ConvertAnim("Pirate_Salsa.glb", "Pirate_Salsa");  

	Azul::ConvertSkin("Ward_Wave.glb", "Ward");
	Azul::ConvertJoint("Ward_Wave.glb", "Ward");
	Azul::ConvertHierarchy("Ward_Wave.glb", "Ward");
	Azul::ConvertSkeleton("Ward_Wave.glb", "Ward");
	Azul::CreateTexturePNG("Ward_Diffuse.png", "Ward");
	Azul::ConvertAnim("Ward_Wave.glb", "Ward_Wave");

	Azul::ConvertJoint("Mousey_Mesh.glb", "Mousey");  
	Azul::ConvertSkin("Mousey_Mesh.glb", "Mousey");  
	Azul::ConvertSkeleton("Mousey_Mesh.glb", "Mousey");  
	Azul::ConvertHierarchy("Mousey_Mesh.glb", "Mousey");  
	Azul::ConvertAnim("Mousey_Anim_Silly_Dancing.glb", "Mousey_SillyDancing");  
	Azul::CreateTexturePNG("Mousey.png", "Mousey");  
	Azul::ConvertAnim("Mousey_Anim_Run_Forward.glb", "Mousey_Run");  
	Azul::ConvertAnim("Mousey_Anim_Gangnam_Style.glb", "Mousey_Gangnam");  


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
