#ifndef BUFFER_VBV_H
#define BUFFER_VBV_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "ShaderMappings.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API BufferVBV_ia
	{
	public:
		BufferVBV_ia();
		BufferVBV_ia(const BufferVBV_ia &r) = delete;
		BufferVBV_ia &operator=(const BufferVBV_ia &) = delete;
		virtual ~BufferVBV_ia();

		void Initialize(uint32_t numBytes, uint32_t _strideSize, void *pData);

		void SetActive(VertexSlot slot);

	private:
		void privCreate(void *pData);

		// ----------------------
		//  Data
		// ----------------------
	public:
		uint32_t	 numBytes;
		uint32_t	 strideSize;
		uint32_t	 offset;
		ID3D11Buffer *poBufferVBV_ia;
		bool         bCreate;
	};
}

#endif

//--- End of File ---
