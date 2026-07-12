//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef AZUL_RENDER_COMPONENT_H
#define AZUL_RENDER_COMPONENT_H

#include "MathEngine.h"
#include "ComponentId.h"
#include "MaterialKind.h"

namespace Azul
{
	class Mesh;
	class ShaderObject;
	class TextureObject;
	class ComputeBlend;

	// Per-entity 3D render data -- the source of truth for a renderable (the
	// GraphicsObject_* hierarchy is retired). Fields are a superset across the 3D
	// MaterialKinds; the RenderSystem branch for each kind reads the ones it needs.
	// (2D sprites carry a Sprite2DComponent instead; kind==Null draws nothing.)
	struct RenderComponent
	{
		MaterialKind kind;
		bool drawEnable;
		int layer;   // draw order: RenderSystem sorts ascending (lower = drawn first).

		Mesh *pMesh;
		ShaderObject *pShader;
		TextureObject *pTex;          // FlatTexture, SkinLightTexture
		ComputeBlend *pComputeBlend;  // SkinLightTexture
		Vec3 lightColor;              // ConstColorLight, SkinLightTexture, Wireframe(color)
		Vec3 lightPos;                // ConstColorLight, SkinLightTexture
		Vec3 bodyColor;               // ConstColorLight
		Mat4 uvMatrix;                // FlatTexture

		static const unsigned int kTypeId = COMPONENT_RENDER;
	};
}

#endif

// --- End of File ---
