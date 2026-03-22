//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WORLD_COMPUTE_A_H
#define WORLD_COMPUTE_A_H

#include "HierarchyTable.h"
#include "Mixer.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"
#include "WorldConstant.h"

namespace Azul
{
	class WorldComputeA
	{
	public:
		WorldComputeA(Mixer *pMixer, HierarchyTable *pHierarchyTable);

		WorldComputeA() = delete;
		WorldComputeA(const WorldComputeA &) = delete;
		WorldComputeA &operator = (const WorldComputeA &) = delete;
		virtual ~WorldComputeA();

		void *GetRawConstBuffer();
		size_t GetNumJoints();

		BufferUAV_cs *GetLocalBone();
		BufferSRV_cs *GetHierarchy();
		BufferUAV_cs *GetUAVWorldMat();
		BufferCBV_cs *GetWorldConstBuffer();
		BufferSRV_cs *GetBoneWorld();

	public:
		// World compute shader data
		BufferUAV_cs *pLocalBone;
		BufferSRV_cs mHierarchy;
		BufferUAV_cs mUAVWorldMat;
		BufferCBV_cs mWorldConstBuffer;

		unsigned int  numJoints;
		unsigned int  hierarchyDepth;
		WorldConstant mWorldConstant;

		// for Skin Vertex shader (can't have a UAV only a SRV)
		BufferSRV_cs mSRVBufferToVS_BoneWorld;


	};
}

#endif

// --- End of File ---
