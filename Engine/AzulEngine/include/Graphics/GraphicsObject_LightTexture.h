//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_LIGHT_TEXTURE_H
#define GRAPHICS_OBJECT_LIGHT_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{

	class GraphicsObject_LightTexture :public GraphicsObject
	{
	public:
		GraphicsObject_LightTexture(Mesh::Name meshName,
			ShaderObject::Name shaderName,
			TextureObject::Name texName,
			Vec3& _pLightColor,
			Vec3& _pLightPos);

		// todo - can this be deleted?
		GraphicsObject_LightTexture(Mesh* pMesh,
			ShaderObject::Name shaderName,
			TextureObject::Name texName,
			Vec3& _pLightColor,
			Vec3& _pLightPos);

		GraphicsObject_LightTexture() = delete;
		GraphicsObject_LightTexture(const GraphicsObject_LightTexture &) = delete;
		GraphicsObject_LightTexture &operator = (const GraphicsObject_LightTexture &) = delete;
		virtual ~GraphicsObject_LightTexture();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here

		TextureObject *pTex;
		Vec3 *poLightColor;
		Vec3 *poLightPos;
	};

}

#endif

// --- End of File ---
