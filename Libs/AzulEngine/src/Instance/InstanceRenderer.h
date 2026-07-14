// NOTE: kept as a GPU-instancing REFERENCE only. This folder is excluded from the
// build in premake5.lua and its shader (ColorInstanced.Vx.hlsl) was removed, so it
// will not compile as-is -- it documents how the instanced draw path was wired.

#ifndef AZUL_INSTANCE_RENDERER_H
#define AZUL_INSTANCE_RENDERER_H

#include "MathEngine.h"
#include "BufferVertexShader_vs.h"
#include "BufferPixelShader_ps.h"
#include "BufferInputLayout_ia.h"
#include "BufferCBV_vs.h"

#include "EngineDLLInterface.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

namespace Azul
{
	class Mesh;
	class Camera;

	// GPU-instanced draw path for the ColorByVertex material. Owns the instanced
	// shader (world per instance from a StructuredBuffer indexed by SV_InstanceID)
	// + View/Proj CBVs + a dynamic per-instance world buffer. Usage:
	//     Mat4 *pDst = InstanceRenderer::BeginUpload(count);
	//     for each instance: pDst[i] = world;
	//     InstanceRenderer::EndUploadAndDraw(pMesh, count, pCam);
	// draws all `count` instances in ONE DrawIndexedInstanced call.
	class AZUL_ENGINE_LIBRARY_API InstanceRenderer
	{
	public:
		// Map the per-instance world buffer (growing it to hold `count`) and return
		// the mapped pointer for the caller to fill with `count` world matrices.
		static Mat4 *BeginUpload(unsigned int count);

		// Unmap, bind the instanced pipeline + mesh, and issue one instanced draw.
		// `color` is the single solid color used for the whole batch.
		static void EndUploadAndDraw(Mesh *pMesh, unsigned int count, Camera *pCam, const Vec3 &color);

		// Release the GPU resources (call from SystemMan::Destroy at scene unload).
		static void Destroy();

	private:
		InstanceRenderer();
		~InstanceRenderer();
		InstanceRenderer(const InstanceRenderer &) = delete;
		InstanceRenderer &operator=(const InstanceRenderer &) = delete;

		static InstanceRenderer *privInstance();   // lazy-create
		void privEnsureCapacity(unsigned int count);

		BufferVertexShader_vs VertexShader;
		BufferPixelShader_ps  PixelShader;
		BufferInputLayout_ia  InputLayout;
		BufferCBV_vs          ConstantBuff_View;
		BufferCBV_vs          ConstantBuff_Projection;
		BufferCBV_vs          ConstantBuff_Color;

		ID3D11Buffer             *poInstanceBuffer;
		ID3D11ShaderResourceView *poInstanceSRV;
		unsigned int              mCapacity;   // in matrices

		static InstanceRenderer *posInstance;
	};
}

#endif

// --- End of File ---
