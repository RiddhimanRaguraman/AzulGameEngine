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
	class GraphicsObject;
	class Mesh;
	class ShaderObject;
	class TextureObject;
	class ComputeBlend;

	// Per-entity render data. Phase 4 consolidation (in progress): RenderComponent
	// is becoming the source of truth for a renderable, so the GraphicsObject_*
	// class hierarchy can be retired. Fields are a superset across MaterialKinds;
	// only the ones a given kind uses are populated (the RenderSystem branch reads
	// them). Migrated 3D materials leave pGraphicsObject == nullptr; not-yet-
	// migrated 3D kinds and the 2D Sprite/Null paths still carry it.
	struct RenderComponent
	{
		MaterialKind kind;
		bool drawEnable;

		// Resolved handles / material params (source of truth once migrated).
		Mesh *pMesh;
		ShaderObject *pShader;
		TextureObject *pTex;          // FlatTexture, SkinLightTexture
		ComputeBlend *pComputeBlend;  // SkinLightTexture
		Vec3 lightColor;              // ConstColorLight, SkinLightTexture, Wireframe(color)
		Vec3 lightPos;                // ConstColorLight, SkinLightTexture
		Vec3 bodyColor;               // ConstColorLight
		Mat4 uvMatrix;                // FlatTexture

		// Bridge: still owns/uses a GraphicsObject for not-yet-migrated materials
		// (Sprite/Null and any un-migrated 3D kind). nullptr for migrated 3D.
		GraphicsObject *pGraphicsObject;

		static const unsigned int kTypeId = COMPONENT_RENDER;
	};
}

#endif

// --- End of File ---
