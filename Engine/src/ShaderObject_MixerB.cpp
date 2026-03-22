//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_MixerB.h"
#include "Mesh.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

// User generated headers... 
#include "MixerB.Cx.h"

namespace Azul
{
	ShaderObject_MixerB::~ShaderObject_MixerB()
	{
	}

	ShaderObject_MixerB::ShaderObject_MixerB(ShaderObject_MixerB::Name _name)
		: ShaderObject(_name),
		ComputeShader{sizeof(g_MixerB_CxShader), (void *)g_MixerB_CxShader}
	{
	}

	void ShaderObject_MixerB::ActivateShader()
	{
		this->ComputeShader.SetActive();
	}

}

// --- End of File ---
