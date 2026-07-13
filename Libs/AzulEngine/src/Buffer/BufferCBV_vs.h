#ifndef BUFFER_CBV_VS_H
#define BUFFER_CBV_VS_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API BufferCBV_vs
	{
	public:
		BufferCBV_vs();
		BufferCBV_vs(const BufferCBV_vs &r) = delete;
		BufferCBV_vs &operator=(const BufferCBV_vs &) = delete;
		virtual ~BufferCBV_vs();

		BufferCBV_vs(uint32_t structSize);
		void Initialize(uint32_t structSize);

		void Transfer(void *pBuff);
		void SetActive(ConstantVSBufferSlot slot);

	private:
		void privCreate(uint32_t structSize);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t     structSize;
		ID3D11Buffer *poBufferCBV;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
