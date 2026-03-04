//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_PROTO_H
#define MESH_PROTO_H

#include "Mesh.h"
#include "BufferIBV.h"
#include "BufferVBV.h"

namespace Azul
{
	class MeshProto : public Mesh
	{
	public:
		// Data
		MeshProto(const char *const pMeshFileName);

		MeshProto() = delete;
		MeshProto(const MeshProto &) = delete;
		MeshProto &operator = (MeshProto &) = delete;
		virtual ~MeshProto();

		virtual void ActivateMesh() override;
		virtual void RenderIndexBuffer() override;
		virtual void ActivateConstantBuffers() override;
		virtual void Initialize_SkinBoneWorldBuffer(size_t _structSize) override;
		virtual void Transfer_SkinBoneWorldBuffer(Mat4* p) override;
	private:
		// Data
		BufferVBV VBVBuffer_pos;
		BufferVBV VBVBuffer_color;
		BufferVBV VBVBuffer_tex;
		BufferVBV VBVBuffer_norm;

		BufferVBV VBVBuffer_weight;
		BufferVBV VBVBuffer_joint;

		BufferIBV IBVBuffer;

		// Not part of VertexBuffer.. but part of the Mesh
		BufferCBV_vs CBVBuffer_InvBind;
		BufferCBV_vs CBVBuff_SkinBoneWorld;

	};
}

#endif

// --- End of File ---
