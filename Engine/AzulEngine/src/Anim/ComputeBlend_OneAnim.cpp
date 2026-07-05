//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ComputeBlend_OneAnim.h"
#include "Anim.h"
#include "HierarchyTableMan.h"
#include "StateDirectXMan.h"
#include "ShaderObjectNodeMan.h"
#include "Clip.h"

namespace Azul
{
	ComputeBlend_OneAnim::ComputeBlend_OneAnim(Anim* pAnim)
	{
		assert(pAnim);
		Clip* pClip = pAnim->GetClip();
		assert(pClip);

		// Setup Compute shaders data
		this->poMixerA = new MixerA(pClip);
		assert(this->poMixerA);

		HierarchyTable* pHierarchyTable = HierarchyTableMan::Find(pClip->GetHierarchyName());
		assert(pHierarchyTable);

		this->poWorldComputeA = new WorldComputeA(this->poMixerA, pHierarchyTable);
		assert(this->poWorldComputeA);


	}

	ComputeBlend_OneAnim::~ComputeBlend_OneAnim()
	{
		delete this->poMixerA;
		this->poMixerA = nullptr;

		delete this->poWorldComputeA;
		this->poWorldComputeA = nullptr;
	}

	void ComputeBlend_OneAnim::Execute()
	{
		this->privMixerExecute();
		this->privWorldComputeExecute();
	}

	void ComputeBlend_OneAnim::AnimateMixerA(Clip* pClip, AnimTime time)
	{
		pClip->AnimateBones(time, this->poMixerA);
	}

	void ComputeBlend_OneAnim::BindWorldBoneArray()
	{
		BufferSRV_cs* pBoneWorld = this->poWorldComputeA->GetBoneWorld();
		pBoneWorld->BindComputeToVS(ShaderResourceBufferSlot::BoneWorldIn);
	}

	void ComputeBlend_OneAnim::privMixerExecute()
	{
		// ------------------------------------------------
		//  execute Compute Shader 
		// ------------------------------------------------

		ShaderObject* pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::MixerACompute);
		pShaderObj->ActivateShader();

		assert(poMixerA->pKeyAa);
		poMixerA->pKeyAa->BindComputeToCS(ShaderResourceBufferSlot::KeyAa);

		assert(poMixerA->pKeyAb);
		poMixerA->pKeyAb->BindComputeToCS(ShaderResourceBufferSlot::KeyAb);

		assert(poMixerA->GetMixerResult());
		poMixerA->GetMixerResult()->BindCompute(UnorderedAccessBufferSlot::MixerOutAx);

		assert(poMixerA->GetMixerConstBuff());
		poMixerA->GetMixerConstBuff()->Transfer(poMixerA->GetRawConstBuffer());
		poMixerA->GetMixerConstBuff()->BindCompute(ConstantCSBufferSlot::csMixerA);

		// Dispatch - BANANA - adjust dispatch/shader numbers
		unsigned int count = (unsigned int)ceil((float)poMixerA->GetNumNodes() / 1.0f);
		StateDirectXMan::GetContext()->Dispatch(count, 1, 1);
	}

	void ComputeBlend_OneAnim::privWorldComputeExecute()
	{
		// ------------------------------------------------
		//  execute Compute Shader 
		// ------------------------------------------------

		ShaderObject* pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::WorldComputeA);
		pShaderObj->ActivateShader();

		assert(poWorldComputeA->GetLocalBone());
		poWorldComputeA->GetLocalBone()->BindCompute(UnorderedAccessBufferSlot::MixerOutAx);

		assert(poWorldComputeA->GetHierarchy());
		poWorldComputeA->GetHierarchy()->BindComputeToCS(ShaderResourceBufferSlot::HierarchyTable);

		assert(poWorldComputeA->GetUAVWorldMat());
		poWorldComputeA->GetUAVWorldMat()->BindCompute(UnorderedAccessBufferSlot::BoneWorldOut);

		assert(poWorldComputeA->GetWorldConstBuffer());
		poWorldComputeA->GetWorldConstBuffer()->Transfer(poWorldComputeA->GetRawConstBuffer());
		poWorldComputeA->GetWorldConstBuffer()->BindCompute(ConstantCSBufferSlot::csWorld);

		// Dispatch - BANANA - adjust dispatch/shader numbers
		unsigned int count = (unsigned int)ceil((float)poWorldComputeA->GetNumJoints() / 1.0f);
		StateDirectXMan::GetContext()->Dispatch(count, 1, 1);

		// UAV buffers are not allowed in Vertex shaders
		// so copy the UAV buffer into an SRV buffer
		assert(poWorldComputeA->GetBoneWorld());
		StateDirectXMan::GetContext()->CopyResource(poWorldComputeA->GetBoneWorld()->GetD3DBuffer(),
			poWorldComputeA->GetUAVWorldMat()->GetD3DBuffer());

	}


}

// --- End of File ---
