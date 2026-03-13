//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_BASIC_COMPUTE_H
#define SHADER_OBJECT_BASIC_COMPUTE_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_BasicCompute : public ShaderObject
	{
	public:
		ShaderObject_BasicCompute() = delete;
		ShaderObject_BasicCompute(const ShaderObject_BasicCompute &) = delete;
		ShaderObject_BasicCompute &operator = (const ShaderObject_BasicCompute &) = delete;
		virtual ~ShaderObject_BasicCompute();

		ShaderObject_BasicCompute(ShaderObject_BasicCompute::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
