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
	// P5.0: sorted pool pass over all 3D renderables. Gathers the drawEnable 3D
	// entities, stable insertion-sorts them by layer (ascending), and draws each
	// via its MaterialKind branch. No <algorithm> per the coding standard.
	//
	// P6.1 batching: the shader + its constant buffer are bound ONCE per contiguous
	// run of same-shader draws instead of once per object. Re-binding the same
	// shader is idempotent, so this is visually identical -- it just drops the
	// redundant ActivateShader/ActivateCBV binds (e.g. scene1's dancers all share
	// SkinLightTexture -> 1 shader bind instead of 7; a same-shader run even spans
	// layers). Per-object mesh/texture/rasterizer/blend/transform binds are unchanged.
	void RenderSystem::Draw(World &world)
	{
		ComponentPool<RenderComponent> &pool = world.Pool<RenderComponent>();
		const unsigned int count = pool.GetCount();

		const unsigned int MAX_RENDERABLES = 4096;
		unsigned int order[MAX_RENDERABLES];
		unsigned int n = 0;

		for (unsigned int i = 0; i < count; i++)
		{
			RenderComponent &r = pool.GetData(i);
			if (r.drawEnable && MaterialKindIs3D(r.kind))
			{
				assert(n < MAX_RENDERABLES);
				order[n] = i;
				n++;
			}
		}

		// Stable insertion sort by layer (ascending). n is small (tens-hundreds).
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

		// Draw, hoisting the shader/CBV bind across same-shader runs.
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

	// (was GraphicsObject_ColorByVertex::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: reads mesh/shader from RenderComponent (no GraphicsObject).
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

	// (was GraphicsObject_FlatTexture::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: mesh/shader/tex/uvMatrix from RenderComponent.
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

	// (was GraphicsObject_ConstColorLight::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: params from RenderComponent.
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

	// (was GraphicsObject_Wireframe::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: color from RenderComponent.lightColor.
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

	// (was GraphicsObject_SkinLightTexture::SetState/SetDataGPU/Draw/RestoreState)
	// MIGRATED to data-path: mesh/shader/tex/light/ComputeBlend from RenderComponent.
	// Consumes the SkinningSystem output: BindWorldBoneArray binds the bone-world
	// SRV that ComputeBlend::Execute produced during Update (runs before Draw).
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
