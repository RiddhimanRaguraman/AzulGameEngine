//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "WorldComputeA.h"
#include "File.h"
#include "HierarchyTableMan.h"
#include "HierarchyData.h"
#include "MathEngine.h"
#include "MixerA.h"
#include "BufferCBV_cs.h"
#include "BufferSRV_cs.h"
#include "BufferUAV_cs.h"

namespace Azul
{
	WorldComputeA::WorldComputeA(Mixer *pMixer, HierarchyTable *pHierarchyTable)
		:
		pLocalBone{pMixer->GetMixerResult()},
		mHierarchy{pHierarchyTable->GetDepth() * pHierarchyTable->GetNumJoints(), sizeof(unsigned int)},
		mUAVWorldMat{pHierarchyTable->GetNumJoints(), sizeof(Mat4)},
		mWorldConstBuffer{sizeof(WorldConstant)},
		numJoints{(uint32_t)pHierarchyTable->GetNumJoints()},
		hierarchyDepth{(uint32_t)pHierarchyTable->GetDepth()},
		mWorldConstant{0},
		mSRVBufferToVS_BoneWorld{ pHierarchyTable->GetNumJoints(), sizeof(Mat4) }
	{
		assert(pMixer);

		// 1) pLocalBone
		//        comes from Mixer Stage
		//        This is a UAV - ready to go
		//        count... is the numNodes

		// 2) mHierarchy
		//        create a SRV hierarchy table
		//        Transfer data now resides on GPU
		//        This SRV - ready to go
		//        count... is the numJoints
		mHierarchy.Transfer(pHierarchyTable->poHierarchyTable);

		// 3) mUAVWorldMat
		//        create UAV world matrix
		//        World shader will fill this in
		//        count... is the numJoints

		// 4) mWorldConstBuffer
		//        world compute shader needs constant buffer
		//        Create the space for a compute shader
		//        data is transferred during rendering
		//        holds hierchyDepth, numJoints

		// 5) numJoints
		//       num of rows in hierarchy table

		// 6) hierarchy depth
		//       num of columns in hierarchy table

		// 7) mWorldConstant
		//        world constant struct
		//        locally stored data
		//        holds hierchyDepth, numJoints

		mWorldConstant.hierarchyDepth = this->hierarchyDepth;
		mWorldConstant.numJoints = this->numJoints;

		// 8) mBoneWorld
		//        For the Vertex shader... 
		//        it can only work with SRV not UAV
		//        So copy the UAV into the SRV matrix array\
		//        count... is the numJoints

	}

	WorldComputeA::~WorldComputeA()
	{
		// nothing since the buffers are objects
	}

	BufferUAV_cs *WorldComputeA::GetLocalBone()
	{
		return this->pLocalBone;
	}

	BufferSRV_cs *WorldComputeA::GetHierarchy()
	{
		return &this->mHierarchy;
	}

	BufferUAV_cs *WorldComputeA::GetUAVWorldMat()
	{
		return &this->mUAVWorldMat;
	}

	BufferCBV_cs *WorldComputeA::GetWorldConstBuffer()
	{
		return &this->mWorldConstBuffer;
	}

	BufferSRV_cs *WorldComputeA::GetBoneWorld()
	{
		return &this->mSRVBufferToVS_BoneWorld;
	}

	size_t WorldComputeA::GetNumJoints()
	{
		return this->numJoints;
	}

	void *WorldComputeA::GetRawConstBuffer()
	{
		static WorldConstant tmp{0};

		tmp.hierarchyDepth = this->hierarchyDepth;
		tmp.numJoints = this->numJoints;

		return &tmp;
	}

}

// --- End of File ---
