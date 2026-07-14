#include <d3d11.h>

#include "InstanceRenderer.h"
#include "ShaderMappings.h"
#include "StateDirectXMan.h"
#include "SafeRelease.h"
#include "Mesh.h"
#include "Camera.h"
#include "Profiler.h"

// Compiled shaders: the instanced VS (new) reuses the ColorByVertex pixel shader.
#include "ColorInstanced.Vx.h"
#include "ColorByVertex.Px.h"

namespace Azul
{
	InstanceRenderer *InstanceRenderer::posInstance = nullptr;

	// POSITION-only input layout (works with any mesh). The per-instance world
	// comes from the StructuredBuffer via SV_InstanceID and the color from a CBV,
	// so neither is an input-layout element.
	static D3D11_INPUT_ELEMENT_DESC InstanceRendererDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	InstanceRenderer::InstanceRenderer()
		: VertexShader{ sizeof(g_ColorInstanced_VxShader), (void *)g_ColorInstanced_VxShader },
		PixelShader{ sizeof(g_ColorByVertex_PxShader), (void *)g_ColorByVertex_PxShader },
		InputLayout{ sizeof(g_ColorInstanced_VxShader), (void *)g_ColorInstanced_VxShader, InstanceRendererDesc, sizeof(InstanceRendererDesc) },
		ConstantBuff_View{ sizeof(Mat4) },
		ConstantBuff_Projection{ sizeof(Mat4) },
		ConstantBuff_Color{ sizeof(Vec4) },
		poInstanceBuffer(nullptr),
		poInstanceSRV(nullptr),
		mCapacity(0)
	{
	}

	InstanceRenderer::~InstanceRenderer()
	{
		SafeRelease(this->poInstanceSRV);
		SafeRelease(this->poInstanceBuffer);
	}

	InstanceRenderer *InstanceRenderer::privInstance()
	{
		if (posInstance == nullptr)
		{
			posInstance = new InstanceRenderer();
		}
		return posInstance;
	}

	void InstanceRenderer::Destroy()
	{
		delete posInstance;
		posInstance = nullptr;
	}

	void InstanceRenderer::privEnsureCapacity(unsigned int count)
	{
		if (count <= this->mCapacity)
		{
			return;
		}

		// Grow: release the old buffer/SRV and recreate at the new size.
		SafeRelease(this->poInstanceSRV);
		SafeRelease(this->poInstanceBuffer);

		ID3D11Device *pDevice = StateDirectXMan::GetDevice();
		assert(pDevice);

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = count * (uint32_t)sizeof(Mat4);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bd.StructureByteStride = (uint32_t)sizeof(Mat4);

		HRESULT hr = pDevice->CreateBuffer(&bd, nullptr, &this->poInstanceBuffer);
		assert(SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC sd = {};
		sd.Format = DXGI_FORMAT_UNKNOWN;
		sd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		sd.Buffer.FirstElement = 0;
		sd.Buffer.NumElements = count;

		hr = pDevice->CreateShaderResourceView(this->poInstanceBuffer, &sd, &this->poInstanceSRV);
		assert(SUCCEEDED(hr));

		this->mCapacity = count;
	}

	Mat4 *InstanceRenderer::BeginUpload(unsigned int count)
	{
		assert(count > 0);
		InstanceRenderer *pInst = InstanceRenderer::privInstance();
		pInst->privEnsureCapacity(count);

		ID3D11DeviceContext *pContext = StateDirectXMan::GetContext();
		assert(pContext);

		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = pContext->Map(pInst->poInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		assert(SUCCEEDED(hr));

		return (Mat4 *)mapped.pData;
	}

	void InstanceRenderer::EndUploadAndDraw(Mesh *pMesh, unsigned int count, Camera *pCam, const Vec3 &color)
	{
		assert(pMesh);
		assert(pCam);
		InstanceRenderer *pInst = InstanceRenderer::privInstance();

		ID3D11DeviceContext *pContext = StateDirectXMan::GetContext();
		assert(pContext);

		pContext->Unmap(pInst->poInstanceBuffer, 0);

		// Bind the instanced pipeline.
		pInst->VertexShader.SetActive();
		pInst->InputLayout.SetActive();
		pInst->PixelShader.SetActive();

		pInst->ConstantBuff_View.Transfer(&pCam->getViewMatrix());
		pInst->ConstantBuff_Projection.Transfer(&pCam->getProjMatrix());
		pInst->ConstantBuff_View.SetActive(ConstantVSBufferSlot::vsView);
		pInst->ConstantBuff_Projection.SetActive(ConstantVSBufferSlot::vsProjection);

		// Single solid color for the whole batch (b2 -- the vsWorld CBV slot).
		Vec4 col(color.x(), color.y(), color.z(), 1.0f);
		pInst->ConstantBuff_Color.Transfer(&col);
		pInst->ConstantBuff_Color.SetActive(ConstantVSBufferSlot::vsWorld);

		// Per-instance world matrices -> VS SRV register(t0).
		pContext->VSSetShaderResources(0, 1, &pInst->poInstanceSRV);

		pMesh->ActivateMesh();

		Profiler::CountDraw();   // one instanced draw call for the whole batch
		pMesh->RenderIndexBufferInstanced(count);

		// Unbind the SRV so it can be re-mapped next frame without a hazard warning.
		ID3D11ShaderResourceView *pNullSRV = nullptr;
		pContext->VSSetShaderResources(0, 1, &pNullSRV);
	}
}

// --- End of File ---
