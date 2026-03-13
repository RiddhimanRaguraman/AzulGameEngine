//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_Mixer.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "Mixer.Cx.h"

namespace Azul
{
	ShaderObject_Mixer::~ShaderObject_Mixer()
	{
	}

	ShaderObject_Mixer::ShaderObject_Mixer(ShaderObject_Mixer::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_Mixer_CxShader),
					  (void *)g_Mixer_CxShader}
	{
	}

	void ShaderObject_Mixer::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
