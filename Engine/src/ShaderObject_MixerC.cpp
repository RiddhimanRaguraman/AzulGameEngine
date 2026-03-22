//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_MixerC.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "MixerC.Cx.h"

namespace Azul
{
	ShaderObject_MixerC::~ShaderObject_MixerC()
	{
	}

	ShaderObject_MixerC::ShaderObject_MixerC(ShaderObject_MixerC::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_MixerC_CxShader), (void *)g_MixerC_CxShader}
	{
	}

	void ShaderObject_MixerC::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
