#ifndef MIXER_C_H
#define MIXER_C_H
#include "EngineDLLInterface.h"

#include "Mixer.h"


namespace Azul
{
	class Clip;
	class BufferSRV_cs;

	class AZUL_ENGINE_LIBRARY_API MixerC : public Mixer
	{
	public:
		MixerC(Clip *pClip);

		MixerC() = delete;
		MixerC(const MixerC &) = delete;
		MixerC &operator = (const MixerC &) = delete;
		virtual ~MixerC();

	public:
		BufferUAV_cs  *pKeyCa;
		BufferUAV_cs  *pKeyCb;
	};
}

#endif

// --- End of File ---
