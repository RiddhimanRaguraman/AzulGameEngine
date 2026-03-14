//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Mixer.h"
#include "MathApp.h"
#include "ShaderObjectNodeMan.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"

namespace Azul
{
	struct MixerConstant
	{
		float ts;
		unsigned int numNodes;
		int pad0;
		int pad1;
	};


	void Mixer::Blend(Bone* pResult,
		const Bone* pA,
		const Bone* pB,
		const float tS,
		int numNodes)
	{

		assert(pA);
		assert(pB);
		assert(pResult);

		const Bone* pBoneOrigA = pA;
		const Bone* pBoneOrigB = pB;
		Bone* pBoneOrigResult = pResult;

		// Interpolate to tS time, for all bones
		for (int i = 0; i < numNodes; i++)
		{
			// interpolate ahoy!
			Vec3App::Lerp(pResult->T, pA->T, pB->T, tS);
			QuatApp::Slerp(pResult->Q, pA->Q, pB->Q, tS);
			Vec3App::Lerp(pResult->S, pA->S, pB->S, tS);

			// advance the pointer
			pA++;
			pB++;
			pResult++;
		}



		// Let's try the shader to prove out the mixer
		unsigned int uNumBones = (unsigned int)numNodes;

		// ------------------------------------
		// Load SSBO into the shader - 
		//    note: this is only for test
		//          shouldn't be here at all...
		// -----------------------------------
		Bone* pBoneResult = new Bone[uNumBones]();
		Bone* pBoneA = new Bone[uNumBones]();
		Bone* pBoneB = new Bone[uNumBones]();

		// Fill in BoneA & BoneB array
		for (unsigned int i = 0; i < uNumBones; i++)
		{
			pBoneA[i] = pBoneOrigA[i];
			pBoneB[i] = pBoneOrigB[i];
		}

		// ------------------------------------------------
		//  Compute Shader Test
		// ------------------------------------------------

		ShaderObject* pShaderObj = ShaderObjectNodeMan::Find(ShaderObject::Name::MixerCompute);
		pShaderObj->ActivateShader();

		BufferSRV_cs rsvBoneA(uNumBones, sizeof(Bone));
		rsvBoneA.Transfer(pBoneA);
		rsvBoneA.BindCompute(ShaderResourceBufferSlot::KeyA);

		BufferSRV_cs rsvBoneB(uNumBones, sizeof(Bone));
		rsvBoneB.Transfer(pBoneB);
		rsvBoneB.BindCompute(ShaderResourceBufferSlot::KeyB);

		BufferUAV_cs uavBoneResult(uNumBones, sizeof(Bone));
		uavBoneResult.Transfer(pBoneResult);
		uavBoneResult.BindCompute(UnorderedAccessBufferSlot::MixerABOut);

		delete[] pBoneA;
		delete[] pBoneB;
		delete[] pBoneResult;

		BufferCBV_cs cbvMixer(sizeof(MixerConstant));
		MixerConstant mixerData;
		mixerData.ts = tS;
		mixerData.numNodes = numNodes;
		cbvMixer.Transfer(&mixerData);
		cbvMixer.BindCompute(ConstantCSBufferSlot::csMixer);

		// Dispatch
		StateDirectXMan::GetContext()->Dispatch(((unsigned int)numNodes), 1, 1);


		// Block Waiting for stages to complete
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };

		HRESULT hr;
		hr = StateDirectXMan::GetContext()->Map(uavBoneResult.poComputeUAVBuffer,
			0,
			D3D11_MAP_READ,
			0,
			&MappedResource);
		assert(SUCCEEDED(hr));
		assert(MappedResource.pData);

	/*	Bone* p = (Bone*)MappedResource.pData;


		Trace::out("\n\n");
		Trace::out("----------------------------------\n");
		for (unsigned int i = 0; i < uNumBones; i++)
		{
			Bone* pCompute = &p[i];
			Bone* pCpp = &pBoneOrigResult[i];

			Trace::out("\n");
			Trace::out("[%2d]\n", i);
			Trace::out("  T: %+ 5.3f %+ 5.3f %+ 5.3f             %+ 5.3f %+ 5.3f %+ 5.3f  \n",
				pCompute->T[x], pCompute->T[y], pCompute->T[z],
				pCpp->T[x], pCpp->T[y], pCpp->T[z]);
			Trace::out("  Q: %+ 5.3f %+ 5.3f %+ 5.3f %+ 5.3f      %+ 5.3f %+ 5.3f %+ 5.3f %+ 5.3f \n",
				pCompute->Q[x], pCompute->Q[y], pCompute->Q[z], pCompute->Q[w],
				pCpp->Q[x], pCpp->Q[y], pCpp->Q[z], pCpp->Q[w]);
			Trace::out("  S: %+ 5.3f %+ 5.3f %+ 5.3f             %+ 5.3f %+ 5.3f %+ 5.3f \n",
				pCompute->S[x], pCompute->S[y], pCompute->S[z],
				pCpp->S[x], pCpp->S[y], pCpp->S[z]);
		}*/
		StateDirectXMan::GetContext()->Unmap(uavBoneResult.poComputeUAVBuffer, 0);




	}

}

// --- End of File ---