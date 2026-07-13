#ifndef TEXTURE_OBJECT_H
#define TEXTURE_OBJECT_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "BufferSamplerState_ps.h"
#include "BufferTextureSRV_ps.h"
#include "BufferTexture2D.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API TextureObject
	{
	public:
		enum class Name
		{
			NullTexture,
			Duckweed,
			Brick,
			Stone,
			Rocks,
			Crate,
			ChickenBot,
			SpaceFrigate,
			R2D2_BASE,
			DogBot,
			Mousey,
			Crownboi,
			Ward,
			Pirate,
			Maw,
			Drax,
			SpiderBot,
			Halo,
			Terrain,
			SkyBox,
			Test0,
			Test1,
			Test2,
			Test3,
			Stitch,
			FontAriel36,

			Uninitialized
		};

	public:
		TextureObject() = default;
		TextureObject(const TextureObject &) = delete;
		TextureObject &operator = (const TextureObject &) = delete;
		virtual ~TextureObject();

		TextureObject(const char *const pMeshFileName,
					  D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR);

		void ActivateTexture();

		bool HasAlpha() const;

		char *GetName();
		void SetName(TextureObject::Name _name);

	public:


		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		TextureObject::Name mName;

		BufferTextureSRV_ps       SRVTexBuffer;
		BufferSamplerState_ps SamplerStateBuffer;
		BufferTexture2D    Texture2DBuffer;

		unsigned int    width;
		unsigned int    height;
		bool AlphaEnabled;
	};
}

#endif

// --- End of File ---
