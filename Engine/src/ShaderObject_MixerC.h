//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_MIXER_C_H
#define SHADER_OBJECT_MIXER_C_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_MixerC : public ShaderObject
	{
	public:
		ShaderObject_MixerC() = delete;
		ShaderObject_MixerC(const ShaderObject_MixerC &) = delete;
		ShaderObject_MixerC &operator = (const ShaderObject_MixerC &) = delete;
		virtual ~ShaderObject_MixerC();

		ShaderObject_MixerC(ShaderObject_MixerC::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
