//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_WORLD_C_H
#define SHADER_OBJECT_WORLD_C_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_WorldC : public ShaderObject
	{
	public:
		ShaderObject_WorldC() = delete;
		ShaderObject_WorldC(const ShaderObject_WorldC &) = delete;
		ShaderObject_WorldC &operator = (const ShaderObject_WorldC &) = delete;
		virtual ~ShaderObject_WorldC();

		ShaderObject_WorldC(ShaderObject_WorldC::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
