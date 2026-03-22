//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_LIGHT_TEXTURE_H
#define GRAPHICS_OBJECT_SKIN_LIGHT_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureObject.h"
#include "ComputeBlend.h"

namespace Azul
{
	class GraphicsObject_SkinLightTexture :public GraphicsObject
	{
	public:
		GraphicsObject_SkinLightTexture(Mesh::Name meshName,
			ShaderObject::Name shaderName,
			TextureObject::Name textName,
			ComputeBlend* pBlend,
			Vec3& LightColor,
			Vec3& LightPos);

		GraphicsObject_SkinLightTexture() = delete;
		GraphicsObject_SkinLightTexture(const GraphicsObject_SkinLightTexture&) = delete;
		GraphicsObject_SkinLightTexture& operator = (const GraphicsObject_SkinLightTexture&) = delete;
		virtual ~GraphicsObject_SkinLightTexture();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		TextureObject* pTex;
		Vec3* poLightColor;
		Vec3* poLightPos;
		ComputeBlend* poComputeBlend;

	};
}

#endif


// --- End of File ---
