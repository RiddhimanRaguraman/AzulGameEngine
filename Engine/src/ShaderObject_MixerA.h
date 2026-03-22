//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_MIXER_A_H
#define SHADER_OBJECT_MIXER_A_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_MixerA : public ShaderObject
	{
	public:
		ShaderObject_MixerA() = delete;
		ShaderObject_MixerA(const ShaderObject_MixerA &) = delete;
		ShaderObject_MixerA &operator = (const ShaderObject_MixerA &) = delete;
		virtual ~ShaderObject_MixerA();

		ShaderObject_MixerA(ShaderObject_MixerA::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
