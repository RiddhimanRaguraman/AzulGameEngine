//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_SAMPLER_STATE_H
#define BUFFER_SAMPLER_STATE_H

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class BufferSamplerState
	{
	public:
		BufferSamplerState();
		BufferSamplerState(const BufferSamplerState &r) = delete;
		BufferSamplerState &operator=(const BufferSamplerState &) = delete;
		virtual ~BufferSamplerState();

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
