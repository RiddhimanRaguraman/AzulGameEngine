#ifndef MESH_PROTO_H
#define MESH_PROTO_H
#include "EngineDLLInterface.h"

#include "Mesh.h"
#include "BufferIBV_ia.h"
#include "BufferVBV_ia.h"
#include "BufferSRV_cs.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API MeshProto : public Mesh
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
		virtual void ActivateSRVBuffers() override;
	private:
		// Data
		BufferVBV_ia VBVBuffer_pos;
		BufferVBV_ia VBVBuffer_color;
		BufferVBV_ia VBVBuffer_tex;
		BufferVBV_ia VBVBuffer_norm;

		BufferVBV_ia VBVBuffer_weight;
		BufferVBV_ia VBVBuffer_joint;

		BufferIBV_ia IBVBuffer;

		// Not part of VertexBuffer.. but part of the Mesh
		BufferSRV_cs SRVBufferToVS_InvBind;

	};
}

#endif

// --- End of File ---
