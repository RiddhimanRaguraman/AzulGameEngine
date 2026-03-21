//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef WORLD_COMPUTE_H
#define WORLD_COMPUTE_H

#include "HierarchyTable.h"
#include "Mixer.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"

namespace Azul
{
	// structure for Constant Compute buffer needs to be multiple of 16 bytes
	struct WorldConstant
	{
		unsigned int hierarchyDepth;
		unsigned int numJoints;
		int pad0;
		int pad1;
	};

	class WorldCompute
	{
	public:
		WorldCompute(Mixer *pMixer, HierarchyTable *pHierarchyTable);

		WorldCompute() = delete;
		WorldCompute(const WorldCompute &) = delete;
		WorldCompute &operator = (const WorldCompute &) = delete;
		virtual ~WorldCompute();

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
		BufferSRV_cs mBoneWorld;  

	};
}

#endif

// --- End of File ---
