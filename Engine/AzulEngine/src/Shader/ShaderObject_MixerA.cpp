//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_MixerA.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "MixerA.Cx.h"

namespace Azul
{
	ShaderObject_MixerA::~ShaderObject_MixerA()
	{
	}

	ShaderObject_MixerA::ShaderObject_MixerA(ShaderObject_MixerA::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_MixerA_CxShader),(void *)g_MixerA_CxShader}
	{
	}

	void ShaderObject_MixerA::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
