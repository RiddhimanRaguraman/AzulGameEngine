//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "ShaderMappings.h"
#include "Mesh.h"
#include "Anim.h"
#include "GameObjectAnimSkeleton.h"

#include "ShaderObjectNodeMan.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"
#include "HierarchyTableMan.h"

namespace Azul
{

	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *pGraphicsObject, Mixer *_pMixer, WorldCompute *_pWorldCompute)
		: GameObjectControlled(pGraphicsObject),
		poScale{ new Vec3(1.0f, 1.0f, 1.0f) },
		poQuat{ new Quat(0.0f, 0.0f, 0.0f, 1.0f) },
		poTrans{ new Vec3(0.0f, 0.0f, 0.0f) },
		delta_x{ 0.0f },
		delta_y{ 0.0f },
		delta_z{ 0.0f },
		cur_rot_x{ 0.0f },
		cur_rot_y{ 0.0f },
		cur_rot_z{ 0.0f },
		poMixer{ _pMixer },
		poWorldCompute{ _pWorldCompute }
	{
		assert(pGraphicsObject);

		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);

		assert(this->poMixer);
		assert(this->poWorldCompute);
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->poMixer;
		delete this->poWorldCompute;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);


		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);

		// do the compute shaders
		this->privMixerExecute();
		this->privWorldComputeExecute();
	}

	void GameObjectAnimSkin::privMixerExecute()
	{
		// ------------------------------------------------
		//  execute Compute Shader 
		// ------------------------------------------------

		ShaderObject *pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::MixerCompute);
		pShaderObj->ActivateShader();

		assert(poMixer->pKeyA);
		poMixer->pKeyA->BindCompute(ShaderResourceBufferSlot::KeyA);

		assert(poMixer->pKeyB);
		poMixer->pKeyB->BindCompute(ShaderResourceBufferSlot::KeyB);

		assert(poMixer->GetMixerResult());
		poMixer->GetMixerResult()->BindCompute(UnorderedAccessBufferSlot::MixerABOut);

		assert(poMixer->GetMixerConstBuff());
		poMixer->GetMixerConstBuff()->Transfer(poMixer->GetRawConstBuffer());
		poMixer->GetMixerConstBuff()->BindCompute(ConstantCSBufferSlot::csMixer);

		// Dispatch - BANANA - adjust dispatch/shader numbers
		unsigned int count = (unsigned int)ceil((float)poMixer->GetNumNodes() / 1.0f);
		StateDirectXMan::GetContext()->Dispatch(count, 1, 1);

	}

	void GameObjectAnimSkin::privWorldComputeExecute()
	{
		// ------------------------------------------------
		//  execute Compute Shader 
		// ------------------------------------------------

		ShaderObject *pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::WorldCompute);
		pShaderObj->ActivateShader();

		assert(poWorldCompute->GetLocalBone());
		poWorldCompute->GetLocalBone()->BindCompute(UnorderedAccessBufferSlot::MixerABOut);

		assert(poWorldCompute->GetHierarchy());
		poWorldCompute->GetHierarchy()->BindCompute(ShaderResourceBufferSlot::HierarchyTable);

		assert(poWorldCompute->GetUAVWorldMat());
		poWorldCompute->GetUAVWorldMat()->BindCompute(UnorderedAccessBufferSlot::BoneWorldOut);

		assert(poWorldCompute->GetWorldConstBuffer());
		poWorldCompute->GetWorldConstBuffer()->Transfer(poWorldCompute->GetRawConstBuffer());
		poWorldCompute->GetWorldConstBuffer()->BindCompute(ConstantCSBufferSlot::csWorld);

		// Dispatch - BANANA - adjust dispatch/shader numbers
		unsigned int count = (unsigned int)ceil((float)poWorldCompute->GetNumJoints() / 1.0f);
		StateDirectXMan::GetContext()->Dispatch(count, 1, 1);

		// UAV buffers are not allowed in Vertex shaders
		// so copy the UAV buffer into an SRV buffer
		assert(poWorldCompute->GetBoneWorld());
		StateDirectXMan::GetContext()->CopyResource(poWorldCompute->GetBoneWorld()->GetD3DBuffer(),
			poWorldCompute->GetUAVWorldMat()->GetD3DBuffer());

	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		Trans T(*this->poTrans);
		Scale S(*this->poScale);
		Quat  Q(*this->poQuat);

		cur_rot_x += delta_x;
		cur_rot_y += delta_y;
		cur_rot_z += delta_z;

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		// world matrix
		*this->poWorld = S * Q * Rx * Ry * Rz * T;
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnimSkin::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void GameObjectAnimSkin::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnimSkin::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		AZUL_UNUSED_VAR(i);
		// This object doesn't use index like bones do
		assert(false); 
	}
}

// --- End of File ---
