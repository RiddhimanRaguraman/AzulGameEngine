#ifndef STATE_ADAPTER_OUTPUT_H
#define STATE_ADAPTER_OUTPUT_H
#include "EngineDLLInterface.h"

#include <d3d11.h>

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API StateAdapterOutput
	{
	public:
		StateAdapterOutput();
		StateAdapterOutput(const StateAdapterOutput &) = delete;
		StateAdapterOutput &operator = (const StateAdapterOutput &) = delete;
		virtual ~StateAdapterOutput();


		IDXGIOutput *poDXGAdapterOutput;
	};

}
#endif 

// --- End of File ---
