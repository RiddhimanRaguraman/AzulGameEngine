//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_CUBE_COLOR_H
#define MESH_CUBE_COLOR_H
#include "EngineDLLInterface.h"

#include "Mesh.h"
#include "BufferVBV_ia.h"
#include "BufferIBV_ia.h"

namespace Azul
{
	// A code-defined (procedural) unit cube [-1,1]^3 with a UNIQUE color at each
	// of its 8 corners, so the ColorByVertex shader interpolates a gradient across
	// every face. Unlike the proto meshes, this one carries real vertex colors --
	// the loaded CubeMesh.m.proto.azul has none (renders black under ColorByVertex).
	class AZUL_ENGINE_LIBRARY_API MeshCubeColor : public Mesh
	{
	public:
		MeshCubeColor(Mesh::Name name);

		MeshCubeColor() = delete;
		MeshCubeColor(const MeshCubeColor &) = delete;
		MeshCubeColor &operator=(const MeshCubeColor &) = delete;
		virtual ~MeshCubeColor();

		virtual void ActivateMesh() override;
		virtual void RenderIndexBuffer() override;

	private:
		BufferVBV_ia VBVBuffer_pos;
		BufferVBV_ia VBVBuffer_color;
		BufferIBV_ia IBVBuffer;
	};
}

#endif

// --- End of File ---
