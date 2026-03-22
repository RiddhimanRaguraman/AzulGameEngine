//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_MIXER_B_H
#define SHADER_OBJECT_MIXER_B_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_MixerB : public ShaderObject
	{
	public:
		ShaderObject_MixerB() = delete;
		ShaderObject_MixerB(const ShaderObject_MixerB &) = delete;
		ShaderObject_MixerB &operator = (const ShaderObject_MixerB &) = delete;
		virtual ~ShaderObject_MixerB();

		ShaderObject_MixerB(ShaderObject_MixerB::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
