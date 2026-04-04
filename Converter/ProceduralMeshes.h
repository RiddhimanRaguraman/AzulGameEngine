//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PROCEDURAL_MESHES_H
#define PROCEDURAL_MESHES_H

namespace Azul
{
	void createFlatplane(const char* const pTargetName, float len, float urep, float vrep);
	void createSkyBox(const char* const pTargetName, float len, float urep, float vrep);
	void createterrain(const char* const pHeightMapFile, const char* const pTargetName, float len, float maxHeight, float yTrans, int repeatU, int repeatV);

	void CreateFlatPlane(const char* const pTargetName, float len, float urep, float vrep);
	void CreateSkyBox(const char* const pTargetName, float len, float urep, float vrep);
	void CreateTerrain(const char* const pHeightMapFile, const char* const pTargetName, float len, float maxHeight, float yTrans, int repeatU, int repeatV);
}

#endif

// --- End of File ---
