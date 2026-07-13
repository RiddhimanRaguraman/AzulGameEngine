#ifndef AZUL_RENDER_SYSTEM_H
#define AZUL_RENDER_SYSTEM_H

#include "EngineDLLInterface.h"
#include "MathEngine.h"

namespace Azul
{
	class World;
	struct RenderComponent;

	// Draw-phase render dispatch. Owns the per-MaterialKind render logic.
	class AZUL_ENGINE_LIBRARY_API RenderSystem
	{
	public:
		// Draw all 3D renderables from the RenderComponent pool, sorted by `layer`
		// ascending (lower = drawn first, e.g. the alpha skybox before the terrain).
		static void Draw(World &world);

	private:
		// Render one 3D renderable via its MaterialKind branch.
		static void DrawObject(RenderComponent &r, Mat4 &world);

		static void privDrawColorByVertex(RenderComponent &r, Mat4 &world);
		static void privDrawFlatTexture(RenderComponent &r, Mat4 &world);
		static void privDrawConstColorLight(RenderComponent &r, Mat4 &world);
		static void privDrawWireframe(RenderComponent &r, Mat4 &world);
		static void privDrawSkinLightTexture(RenderComponent &r, Mat4 &world);
	};
}

#endif

// --- End of File ---
