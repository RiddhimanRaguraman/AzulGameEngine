#ifndef AZUL_MATERIAL_KIND_H
#define AZUL_MATERIAL_KIND_H

namespace Azul
{
	// Which material/shader path a renderable uses. The RenderSystem (3D) and
	// SpriteRenderSystem (2D) branch on it.
	enum class MaterialKind
	{
		Null,             // root/placeholder -- not drawn
		FlatTexture,      // 3D, textured (perspective)
		ColorByVertex,    // 3D, per-vertex color, unlit (perspective)
		ConstColorLight,  // 3D, constant body color + light (perspective)
		Wireframe,        // 3D, wire rasterizer + constant color (perspective)
		Sprite,           // 2D, textured + alpha (orthographic) -- incl. fonts
		SkinLightTexture  // 3D, skinned + lit (perspective)
	};

	// True for the 3D kinds the RenderSystem draws (vs the 2D Sprite kind / Null root).
	inline bool MaterialKindIs3D(MaterialKind k)
	{
		return k == MaterialKind::FlatTexture
			|| k == MaterialKind::ColorByVertex
			|| k == MaterialKind::ConstColorLight
			|| k == MaterialKind::Wireframe
			|| k == MaterialKind::SkinLightTexture;
	}
}

#endif

// --- End of File ---
