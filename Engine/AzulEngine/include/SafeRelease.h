//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_ENGINE_SAFE_RELEASE_H
#define AZUL_ENGINE_SAFE_RELEASE_H

namespace Azul
{
	// Release a COM / DirectX interface pointer and null it.
	//
	// Coding-standard note (WhatToDo.md 0a): this replaces the old
	//   #define SafeRelease(x) { if(x){ x->Release(); x = 0; } }
	// macro. It is the sanctioned "inherently generic" template exception:
	// it must work for every ID3D11*/IDXGI* pointer type AND null the
	// caller's own pointer (needs T*&), which a plain function cannot do
	// without unsafe casts. Header-only inline -- templates are not
	// dll-exported, so each translation unit compiles its own copy; no
	// AZUL_ENGINE_LIBRARY_API needed.
	template <typename T>
	inline void SafeRelease(T *&p)
	{
		if (p)
		{
			p->Release();
			p = nullptr;
		}
	}
}

#endif

// --- End of File ---
