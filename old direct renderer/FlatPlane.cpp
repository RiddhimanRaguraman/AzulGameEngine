#include "FlatPlane.h"
#include "Model.h"
#include "d3dUtil.h"

FlatPlane::FlatPlane(ID3D11Device* dev, float len, float urep, float vrep)
{
	int nverts = 4;
	StandardVertex *pVerts = new StandardVertex[nverts];

	int ntri = 2;
	TriangleByIndex *pTriList = new TriangleByIndex[ntri];

	// Define the normal for the plane (pointing up along Y-axis)
	Vect normal(0, 1, 0);

	// Define vertices with positions, UVs, normals, and colors
	pVerts[0].set(-0.5f * len, 0, -0.5f * len, urep, vrep, normal, Colors::Black);
	pVerts[1].set(-0.5f * len, 0, 0.5f * len, urep, 0, normal, Colors::Blue);
	pVerts[2].set(0.5f * len, 0, 0.5f * len, 0, 0, normal, Colors::Magenta);
	pVerts[3].set(0.5f * len, 0, -0.5f * len, 0, vrep, normal, Colors::Red);

	// Define triangles (two for the plane)
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// Create the model
	pPlane = new Model(dev, pVerts, nverts, pTriList, ntri);

	// Clean up allocated memory
	delete[] pVerts;
	delete[] pTriList;
}

Model* FlatPlane::GetModel()
{
	return pPlane;
}

FlatPlane::~FlatPlane()
{
	delete pPlane;
}

void FlatPlane::Render(ID3D11DeviceContext* context)
{
	pPlane->SetToContext(context);
	pPlane->Render(context);
}