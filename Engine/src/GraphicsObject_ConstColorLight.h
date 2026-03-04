//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_CONST_COLOR_LIGHT_H
#define GRAPHICS_OBJECT_CONST_COLOR_LIGHT_H

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	

	class GraphicsObject_ConstColorLight :public GraphicsObject
	{
	public:
		GraphicsObject_ConstColorLight( Mesh::Name meshName,
										ShaderObject::Name shaderName,
										Vec3& LightColor,
										Vec3& LightPos,
										Vec3& BodyColor);

		GraphicsObject_ConstColorLight() = delete;
		GraphicsObject_ConstColorLight(const GraphicsObject_ConstColorLight &) = delete;
		GraphicsObject_ConstColorLight &operator = (const GraphicsObject_ConstColorLight &) = delete;
		virtual ~GraphicsObject_ConstColorLight();

		// Rendermaterial contract
		virtual void SetState() override;
		virtual void SetDataGPU() override;
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here

		Vec3 *poLightColor;
		Vec3 *poLightPos;
		Vec3 *poBodyColor;
	};

}

#endif

// --- End of File ---
