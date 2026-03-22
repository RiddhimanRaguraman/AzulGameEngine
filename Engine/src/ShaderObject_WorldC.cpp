//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_WorldC.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "WorldC.Cx.h"

namespace Azul
{
	ShaderObject_WorldC::~ShaderObject_WorldC()
	{
	}

	ShaderObject_WorldC::ShaderObject_WorldC(ShaderObject_WorldC::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_WorldC_CxShader),
					  (void *)g_WorldC_CxShader}
	{
	}

	void ShaderObject_WorldC::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
