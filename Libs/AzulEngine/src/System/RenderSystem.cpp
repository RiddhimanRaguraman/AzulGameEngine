#include "RenderSystem.h"
#include "World.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "ComputeBlend.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "Color.h"
#include "CameraNodeMan.h"
#include "Engine.h"
#include "Profiler.h"

namespace Azul
{
	// Sorted pool pass over all 3D renderables: gathers the drawEnable 3D entities,
	// stable insertion-sorts them by layer (ascending), and draws each via its
	// MaterialKind branch. The shader + its constant buffer are bound once per
	// contiguous run of same-shader draws (re-binding the same shader is idempotent),
	// dropping redundant ActivateShader/ActivateCBV binds.
	void RenderSystem::Draw(World &world)
	{
		ComponentPool<RenderComponent> &pool = world.Pool<RenderComponent>();
		const unsigned int count = pool.GetCount();
		if (count == 0)
		{
			return;
		}

		// Draw order, heap-allocated and sized to the renderable pool so any object
		// count works. One alloc/free per frame, negligible vs the draws.
		unsigned int *order = new unsigned int[count];
		unsigned int n = 0;

		for (unsigned int i = 0; i < count; i++)
		{
			RenderComponent &r = pool.GetData(i);
			if (r.drawEnable && MaterialKindIs3D(r.kind))
			{
				order[n] = i;
				n++;
			}
		}

		// Stable insertion sort by layer (ascending). O(n) when everything shares a
		// layer (the common case, incl. the stress grid); O(n^2) only with many
		// mixed layers -- revisit with a radix/counting sort if that ever matters.
		for (unsigned int a = 1; a < n; a++)
		{
			const unsigned int keyIdx = order[a];
			const int keyLayer = pool.GetData(keyIdx).layer;
			int b = (int)a - 1;
			while (b >= 0 && pool.GetData(order[b]).layer > keyLayer)
			{
				order[b + 1] = order[b];
				b--;
			}
			order[b + 1] = keyIdx;
		}

		// Draw each renderable per object, hoisting the shader/CBV bind across
		// same-shader runs (re-binding the same shader is idempotent).
		ShaderObject *pLastShader = nullptr;
		for (unsigned int k = 0; k < n; k++)
		{
			RenderComponent &r = pool.GetData(order[k]);
			const Entity &e = pool.GetOwner(order[k]);
			TransformComponent *pT = world.TryGet<TransformComponent>(e);
			assert(pT);

			if (r.pShader != pLastShader)
			{
				assert(r.pShader);
				r.pShader->ActivateShader();
				r.pShader->ActivateCBV();
				pLastShader = r.pShader;
			}

			DrawObject(r, pT->world);
		}

		delete[] order;
	}

	// Per-object draw. Assumes the shader + CBV are already bound for r.pShader
	// (RenderSystem::Draw hoists that across same-shader runs).
	void RenderSystem::DrawObject(RenderComponent &r, Mat4 &world)
	{
		Profiler::CountDraw();   // one 3D draw call per object

		switch (r.kind)
		{
		case MaterialKind::ColorByVertex:
			privDrawColorByVertex(r, world);
			break;

		case MaterialKind::FlatTexture:
			privDrawFlatTexture(r, world);
			break;

		case MaterialKind::ConstColorLight:
			privDrawConstColorLight(r, world);
			break;

		case MaterialKind::Wireframe:
			privDrawWireframe(r, world);
			break;

		case MaterialKind::SkinLightTexture:
			privDrawSkinLightTexture(r, world);
			break;

		default:
			// DrawObject is only called for 3D kinds (all have a branch above);
			// Null/Sprite never reach here.
			assert(false);
			break;
		}
	}

	// 3D, per-vertex color, unlit. Reads mesh/shader from the RenderComponent.
	void RenderSystem::privDrawColorByVertex(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		pMesh->ActivateMesh();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);

		pMesh->RenderIndexBuffer();
	}

	// 3D, textured. Reads mesh/shader/tex/uvMatrix from the RenderComponent.
	void RenderSystem::privDrawFlatTexture(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);
		assert(r.pTex);

		// SetState
		r.pTex->ActivateTexture();
		if (r.pTex->HasAlpha())
		{
			Engine::GetInstance()->mBlendStateAlpha.Activate();
		}

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferUVMatrix(&r.uvMatrix);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}

	// 3D, constant body color + a light. Reads its params from the RenderComponent.
	void RenderSystem::privDrawConstColorLight(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		pMesh->ActivateMesh();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferPos(&r.lightPos);
		pShader->TransferColor(&r.lightColor);

		Color col(r.bodyColor);
		pShader->TransferBodyColor(&col);

		pMesh->RenderIndexBuffer();
	}

	// 3D, wire rasterizer + a constant color (from RenderComponent.lightColor).
	void RenderSystem::privDrawWireframe(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);

		// SetState
		Engine::GetInstance()->mStateRasterizerWireNoCull.Activate();

		// SetDataGPU
		pMesh->ActivateMesh();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferColor(&r.lightColor);

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mStateRasterizerSolidCull.Activate();
	}

	// 3D, skinned + lit. Reads mesh/shader/tex/light/ComputeBlend from the
	// RenderComponent. BindWorldBoneArray binds the bone-world SRV that
	// ComputeBlend::Execute (SkinningSystem, runs before Draw) produced.
	void RenderSystem::privDrawSkinLightTexture(RenderComponent &r, Mat4 &world)
	{
		Mesh *pMesh = r.pMesh;
		ShaderObject *pShader = r.pShader;
		assert(pMesh);
		assert(pShader);
		assert(r.pTex);
		assert(r.pComputeBlend);

		// SetState
		r.pTex->ActivateTexture();
		Engine::GetInstance()->mStateRasterizerSolidCull.Activate();

		// SetDataGPU
		pMesh->ActivateMesh();
		pMesh->ActivateSRVBuffers();
		pShader->TransferWorldViewProj(CameraNodeMan::GetCurrent(Camera::Type::PERSPECTIVE_3D), &world);
		pShader->TransferPos(&r.lightPos);
		pShader->TransferColor(&r.lightColor);
		r.pComputeBlend->BindWorldBoneArray();

		// Draw
		pMesh->RenderIndexBuffer();

		// RestoreState
		Engine::GetInstance()->mBlendStateOff.Activate();
	}
}

// --- End of File ---
