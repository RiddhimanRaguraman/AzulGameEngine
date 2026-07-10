//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_FLAT_TEXTURE_H
#define GRAPHICS_OBJECT_FLAT_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureObject.h"

#include "EngineDLLInterface.h"

namespace Azul
{

	class AZUL_ENGINE_LIBRARY_API GraphicsObject_FlatTexture :public GraphicsObject
	{
	public:
		GraphicsObject_FlatTexture(Mesh::Name meshName,
			ShaderObject::Name shaderName,
			TextureObject::Name textName);
		GraphicsObject_FlatTexture() = delete;
		GraphicsObject_FlatTexture(const GraphicsObject_FlatTexture &) = delete;
		GraphicsObject_FlatTexture &operator = (const GraphicsObject_FlatTexture &) = delete;
		virtual ~GraphicsObject_FlatTexture() = default;

		void SetUVRepeat(float uRepeat, float vRepeat);

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;
		virtual MaterialKind GetMaterialKind() const override { return MaterialKind::FlatTexture; }

		// ---------------------
		// Data
		// ---------------------

		TextureObject *pTex;
		Mat4 uvMatrix;

	};

}

#endif


// --- End of File ---
