//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BUFFER_UAV_CS_H
#define BUFFER_UAV_CS_H

#include <d3d11.h>
#include "ShaderMappings.h"
#include "Mesh.h"

namespace Azul
{
	//	struct BufType
	//	{
	//		int i;
	//		float f;
	//	};

	struct BufType
	{
		BufType()
			: GroupID(),
			GroupThreadID(),
			DispatchThreadID(),
			GroupIndex{ 0 }
		{
		};

		Vec3ui GroupID;         // Dispatch
		Vec3ui GroupThreadID;   // Local
		Vec3ui DispatchThreadID; // combo 
		int    GroupIndex;      // flatten index
	};

	class BufferUAV_cs
	{
	public:
		BufferUAV_cs() = delete;
		BufferUAV_cs(const BufferUAV_cs& r) = delete;
		BufferUAV_cs& operator=(const BufferUAV_cs&) = delete;
		virtual ~BufferUAV_cs();

		BufferUAV_cs(size_t count, size_t structSize);

		void Transfer(void* pBuff);
		void BindCompute(UnorderedAccessBufferSlot slot);

	private:
		void privCreate(size_t count, size_t structSize);

		// ----------------------
		//  Data
		// -----------------------
	public:
		size_t count;
		size_t structSize;
		void* pBuff;
		ID3D11Buffer* poComputeUAVBuffer;
		ID3D11UnorderedAccessView* poUnorderedAccessView;
		bool bCreate;
	};
}

#endif

//--- End of File ---