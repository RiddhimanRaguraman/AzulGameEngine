//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "GameMan.h"
#include "ProtoBuf.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	// RAII - initialization
	ProtoBuf protobuf;

	// Game is inside a singleton
	int status = Azul::GameMan::Create("Azul Game Engine",
									   2 * 400, // width
									   2 * 300, // height
									   hInstance,
									   prevInstance,
									   cmdLine,
									   cmdShow);
	// Clean up
	Azul::GameMan::Destroy();



	return status;
}

// --- End of File ---
