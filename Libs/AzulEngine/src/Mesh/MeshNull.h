#ifndef MESH_NULL_H
#define MESH_NULL_H
#include "EngineDLLInterface.h"

#include "Mesh.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API MeshNull : public Mesh
	{
	public:
		MeshNull();
		MeshNull(const MeshNull &) = delete;
		MeshNull &operator=(const MeshNull &other) = delete;
		virtual ~MeshNull();

		virtual void ActivateMesh() override;
		virtual void RenderIndexBuffer() override;

	private:

	};
}

#endif

// --- End of File ---
