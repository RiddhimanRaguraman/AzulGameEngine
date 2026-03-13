//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_SAMPLER_STATE_H
#define BUFFER_SAMPLER_STATE_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferSamplerState_ps
	{
	public:
		BufferSamplerState_ps();
		BufferSamplerState_ps(const BufferSamplerState_ps &r) = delete;
		BufferSamplerState_ps &operator=(const BufferSamplerState_ps &) = delete;
		virtual ~BufferSamplerState_ps();

		void Initialize(D3D11_FILTER filter);
		void SetActive(ShaderSamplerSlot slot);

	private:
		void privCreate();

		// ----------------------
		//  Data
		// ----------------------
	public:
		D3D11_FILTER mFilter;
		ID3D11SamplerState *poSampler;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
