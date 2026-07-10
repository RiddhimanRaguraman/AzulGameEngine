//----------------------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MeshCubeColor.h"
#include "ShaderMappings.h"
#include "StateDirectXMan.h"

namespace Azul
{
	MeshCubeColor::MeshCubeColor(Mesh::Name _name)
		: Mesh(8, 36),
		VBVBuffer_pos(),
		VBVBuffer_color(),
		IBVBuffer()
	{
		this->SetName(_name);

		// 8 corners of a unit cube [-0.5, 0.5]^3 (size 1, centered at the origin --
		// same base extent as the proto CubeMesh, so the object's SetScale controls
		// the final size). Geometry only: no translation/rotation baked in here.
		// Index layout:
		//   0:(-,-,-) 1:(+,-,-) 2:(+,+,-) 3:(-,+,-)   (z = -0.5)
		//   4:(-,-,+) 5:(+,-,+) 6:(+,+,+) 7:(-,+,+)   (z = +0.5)
		const float h = 0.5f;
		float pos[8 * 3] =
		{
			-h, -h, -h,
			 h, -h, -h,
			 h,  h, -h,
			-h,  h, -h,
			-h, -h,  h,
			 h, -h,  h,
			 h,  h,  h,
			-h,  h,  h,
		};

		// A unique RGBA color per corner -> the shader interpolates a gradient.
		float color[8 * 4] =
		{
			1.0f, 0.0f, 0.0f, 1.0f,   // 0 red
			1.0f, 1.0f, 0.0f, 1.0f,   // 1 yellow
			0.0f, 1.0f, 0.0f, 1.0f,   // 2 green
			0.0f, 1.0f, 1.0f, 1.0f,   // 3 cyan
			1.0f, 0.0f, 1.0f, 1.0f,   // 4 magenta
			1.0f, 1.0f, 1.0f, 1.0f,   // 5 white
			0.0f, 0.0f, 1.0f, 1.0f,   // 6 blue
			1.0f, 0.5f, 0.0f, 1.0f,   // 7 orange
		};

		// 12 triangles, each wound CCW as seen from OUTSIDE (outward normal). The
		// engine culls FRONT faces (CULL_FRONT, FrontCounterClockwise=FALSE), so
		// the CCW-from-outside faces are the ones kept/visible.
		uint32_t index[36] =
		{
			// +Z (front)
			4, 5, 6,   4, 6, 7,
			// -Z (back)
			0, 2, 1,   0, 3, 2,
			// +X (right)
			1, 2, 6,   1, 6, 5,
			// -X (left)
			0, 4, 7,   0, 7, 3,
			// +Y (top)
			3, 6, 2,   3, 7, 6,
			// -Y (bottom)
			0, 1, 5,   0, 5, 4,
		};

		VBVBuffer_pos.Initialize((uint32_t)sizeof(pos), (uint32_t)(3 * sizeof(float)), pos);
		VBVBuffer_color.Initialize((uint32_t)sizeof(color), (uint32_t)(4 * sizeof(float)), color);
		IBVBuffer.Initialize((uint32_t)sizeof(index), index);
	}

	MeshCubeColor::~MeshCubeColor()
	{
	}

	void MeshCubeColor::ActivateMesh()
	{
		this->VBVBuffer_pos.SetActive(VertexSlot::Position);
		this->VBVBuffer_color.SetActive(VertexSlot::Color);
	}

	void MeshCubeColor::RenderIndexBuffer()
	{
		this->IBVBuffer.SetActive();
		StateDirectXMan::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		StateDirectXMan::GetContext()->DrawIndexed(this->numIndices, 0, 0);
	}
}

// --- End of File ---
