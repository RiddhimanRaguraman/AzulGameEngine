//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_Null.h"

namespace Azul
{
	GraphicsObject_Null::GraphicsObject_Null(Mesh::Name meshName,
		ShaderObject::Name shaderName)
		: GraphicsObject(meshName, shaderName)
	{

	}

	void GraphicsObject_Null::SetState()
	{

	}

	void GraphicsObject_Null::SetDataGPU()
	{

	}

	void GraphicsObject_Null::Draw()
	{

	}

	void GraphicsObject_Null::RestoreState()
	{

	}
}

// ---  End of File ---
