//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_NULL_H
#define GRAPHICS_OBJECT_NULL_H

#include "GraphicsObject.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	class AZUL_ENGINE_LIBRARY_API GraphicsObject_Null :public GraphicsObject
	{
	public:
		GraphicsObject_Null(Mesh::Name meshName,
			ShaderObject::Name shaderName);

		GraphicsObject_Null() = delete;
		GraphicsObject_Null(const GraphicsObject_Null &) = delete;
		GraphicsObject_Null &operator = (const GraphicsObject_Null &) = delete;
		virtual ~GraphicsObject_Null() = default;

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;
		virtual MaterialKind GetMaterialKind() const override { return MaterialKind::Null; }

		// data:  place uniform instancing here

	};

}

#endif

// ---  End of File ---
