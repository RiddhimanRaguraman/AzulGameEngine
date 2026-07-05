//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_WorldA.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "WorldA.Cx.h"

namespace Azul
{
	ShaderObject_WorldA::~ShaderObject_WorldA()
	{
	}

	ShaderObject_WorldA::ShaderObject_WorldA(ShaderObject_WorldA::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_WorldA_CxShader),
					  (void *)g_WorldA_CxShader}
	{
	}

	void ShaderObject_WorldA::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
