//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_BasicCompute.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "BasicCompute.Cx.h"

namespace Azul
{
	ShaderObject_BasicCompute::~ShaderObject_BasicCompute()
	{
	}

	ShaderObject_BasicCompute::ShaderObject_BasicCompute(ShaderObject_BasicCompute::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_BasicCompute_CxShader),
					  (void *)g_BasicCompute_CxShader}
	{
	}

	void ShaderObject_BasicCompute::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
