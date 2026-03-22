//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_WORLD_A_H
#define SHADER_OBJECT_WORLD_A_H

#include "ShaderObject.h"
#include "BufferComputeShader_cs.h"

namespace Azul
{
	class ShaderObject_WorldA : public ShaderObject
	{
	public:
		ShaderObject_WorldA() = delete;
		ShaderObject_WorldA(const ShaderObject_WorldA &) = delete;
		ShaderObject_WorldA &operator = (const ShaderObject_WorldA &) = delete;
		virtual ~ShaderObject_WorldA();

		ShaderObject_WorldA(ShaderObject_WorldA::Name _name);

		virtual void ActivateShader() override;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		BufferComputeShader_cs ComputeShader;
	};
}

#endif

// --- End of File ---
