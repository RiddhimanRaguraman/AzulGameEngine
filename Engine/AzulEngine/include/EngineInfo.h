//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef AZUL_ENGINE_INFO_H
#define AZUL_ENGINE_INFO_H

#include "EngineDLLInterface.h"

namespace Azul
{
	// First public symbol exported from AzulEngine.dll.
	// Exists so the DLL emits an import library and so the app can verify
	// the engine module link path. Real engine API arrives as files migrate
	// into the DLL (see WhatToDo.md Phase 0).
	AZUL_ENGINE_LIBRARY_API const char *GetAzulEngineVersion();
}

#endif

// --- End of File ---
