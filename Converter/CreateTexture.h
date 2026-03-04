//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CREATE_TEXTURE_H
#define CREATE_TEXTURE_H

namespace Azul
{
	void CreateTextureTGA(const char* const pName, const char* const pTargetName);
	void CreateTexturePNG(const char* const pName, const char* const pTargetName);

	// Create texture file from an embedded GLB texture
	void ConvertTextureGLB(const char* const pGLBFileName, const char* const pTargetName);
}
#endif

// --- End of File ---
