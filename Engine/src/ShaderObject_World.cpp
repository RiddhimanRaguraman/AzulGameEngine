//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_World.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h" 

// User generated headers... 
#include "World.Cx.h"

namespace Azul
{
	ShaderObject_World::~ShaderObject_World()
	{
	}

	ShaderObject_World::ShaderObject_World(ShaderObject_World::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_World_CxShader),
					  (void *)g_World_CxShader}
	{
	}

	void ShaderObject_World::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
