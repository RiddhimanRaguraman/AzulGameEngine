#include "ModelTools.h"
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 8;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, Colors::Lime);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, Colors::Yellow);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[4].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, Colors::Cyan);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, Colors::White);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);

	// back face
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// left face
	pTriList[4].set(4, 5, 1);
	pTriList[5].set(4, 1, 0);

	// right face
	pTriList[6].set(3, 2, 6);
	pTriList[7].set(3, 6, 7);

	// top face
	pTriList[8].set(1, 5, 6);
	pTriList[9].set(1, 6, 2);

	// bottom face
	pTriList[10].set(4, 0, 3);
	pTriList[11].set(4, 3, 7);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, +0.5f, 0.0f, 0.375f, 0, 0, 1);
	pVerts[vind + 3].set(+0.5f, -0.5f, +0.5f, 0.0f, 0.625f, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, 0, -1);
	pVerts[vind + 3].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5f, 0.625f, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0.25f, 0.625f, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0.25f, 0.875f, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.5f, 0.875f, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 0.25f, 0.375f, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5f, 0.375f, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.5f, 0.125f, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 0.25f, 0.125f,1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5f, 0.625f, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5f, 0.375f, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0.25f, 0.375f, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 0.25f, 0.625f, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1.0f, 0.625f, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 1.0f, 0.375f, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 5;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	// Base vertices
	pVerts[0].set(0.5f, -0.5f, 0.5f, 0, 0, Vect(0, -1, 0), Colors::Red);    // Front-right
	pVerts[1].set(-0.5f, -0.5f, 0.5f, 0, 0, Vect(0, -1, 0), Colors::Green); // Front-left
	pVerts[2].set(-0.5f, -0.5f, -0.5f, 0, 0, Vect(0, -1, 0), Colors::Blue); // Back-left
	pVerts[3].set(0.5f, -0.5f, -0.5f, 0, 0, Vect(0, -1, 0), Colors::Yellow); // Back-right

	// Apex vertex
	pVerts[4].set(0.0f, 0.5f, 0.0f, 0, 0, Vect(0, 1, 0), Colors::White);   // Apex

	// Base triangles
	pTriList[0].set(0, 1, 2); // Triangle 1
	pTriList[1].set(0, 2, 3); // Triangle 2

	// Side triangles
	pTriList[2].set(0, 1, 4); // Front face
	pTriList[3].set(1, 2, 4); // Left face
	pTriList[4].set(2, 3, 4); // Back face
	pTriList[5].set(3, 0, 4); // Right face
}

void ModelTools::CreateUnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	// Vertex and triangle indices
	int vind = 0; // Vertex index
	int tind = 0; // Triangle index

	// Base vertices
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 1, 0, -1, 0);            // Front-right
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, -1, 0);       // Front-left
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 0, 0, -1, 0);      // Back-left
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 0, 0, -1, 0);       // Back-right

	// Base triangles
	pTriList[tind].set(vind, vind + 1, vind + 2);                   // Triangle 1
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);               // Triangle 2
	vind += 4;
	tind += 2;

	// Apex vertex
	float apexX = 0.0f;
	float apexY = 0.5f;
	float apexZ = 0.0f;

	// Front face
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);             // Bottom-right
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);        // Bottom-left
	pVerts[vind + 2].set(apexX, apexY, apexZ, 0.5f, 0, 0, 0, 1);    // Apex
	pTriList[tind].set(vind, vind + 2, vind + 1);                   // Front face triangle
	vind += 3;
	tind += 1;

	// Back face
	pVerts[vind].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);          // Bottom-left
	pVerts[vind + 1].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);      // Bottom-right
	pVerts[vind + 2].set(apexX, apexY, apexZ, 0.5f, 0, 0, 0, -1);  // Apex
	pTriList[tind].set(vind, vind + 2, vind + 1);                   // Back face triangle
	vind += 3;
	tind += 1;

	// Left face
	pVerts[vind].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);           // Bottom-front
	pVerts[vind + 1].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);      // Bottom-back
	pVerts[vind + 2].set(apexX, apexY, apexZ, 0.5f, 0, -1, 0, 0);   // Apex
	pTriList[tind].set(vind, vind + 2, vind + 1);                   // Left face triangle
	vind += 3;
	tind += 1;

	// Right face
	pVerts[vind].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);            // Bottom-back
	pVerts[vind + 1].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);         // Bottom-front
	pVerts[vind + 2].set(apexX, apexY, apexZ, 0.5f, 0, 1, 0, 0);    // Apex
	pTriList[tind].set(vind, vind + 2, vind + 1);                   // Right face triangle
}

void ModelTools::CreateUnitBoxUniqueTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;  // 4 vertices per face, 6 faces
	pVerts = new StandardVertex[nverts];
	ntri = 12;  // 2 triangles per face
	pTriList = new TriangleByIndex[ntri];

	// Back Face
	pVerts[0].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 0, 0, 1);
	pVerts[1].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, 0, 0, 1);
	pVerts[2].set(-0.5f, -0.5f, +0.5f, 0.0f, 0.375f, 0, 0, 1);
	pVerts[3].set(+0.5f, -0.5f, +0.5f, 0.0f, 0.625f, 0, 0, 1);

	// Front Face
	pVerts[4].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 0, 0, -1);
	pVerts[5].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, 0, 0, -1);
	pVerts[6].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, 0, -1);
	pVerts[7].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, 0, -1);

	// Left Face
	pVerts[8].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 1, 0, 0);
	pVerts[9].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 1, 0, 0);
	pVerts[10].set(+0.5f, -0.5f, +0.5f, 0.25f, 0.875f, 1, 0, 0);
	pVerts[11].set(+0.5f, -0.5f, -0.5f, 0.5f, 0.875f, 1, 0, 0);

	// Right Face
	pVerts[12].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, -1, 0, 0);
	pVerts[13].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, -1, 0, 0);
	pVerts[14].set(-0.5f, -0.5f, -0.5f, 0.5f, 0.125f, -1, 0, 0);
	pVerts[15].set(-0.5f, -0.5f, +0.5f, 0.25f, 0.125f, -1, 0, 0);

	// Top Face
	pVerts[16].set(+0.5f, +0.5f, -0.5f, 0.5f, 0.625f, 0, 1, 0);
	pVerts[17].set(-0.5f, +0.5f, -0.5f, 0.5f, 0.375f, 0, 1, 0);
	pVerts[18].set(-0.5f, +0.5f, +0.5f, 0.25f, 0.375f, 0, 1, 0);
	pVerts[19].set(+0.5f, +0.5f, +0.5f, 0.25f, 0.625f, 0, 1, 0);

	// Bottom face
	pVerts[20].set(+0.5f, -0.5f, +0.5f, 1.0f, 0.625f, 0, -1, 0);
	pVerts[21].set(-0.5f, -0.5f, +0.5f, 1.0f, 0.375f, 0, -1, 0);
	pVerts[22].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.375f, 0, -1, 0);
	pVerts[23].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.625f, 0, -1, 0);

	// Defining triangles
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	pTriList[2].set(6, 5, 4);
	pTriList[3].set(7, 6, 4);

	pTriList[4].set(8, 9, 10);
	pTriList[5].set(8, 10, 11);

	pTriList[6].set(12, 13, 14);
	pTriList[7].set(12, 14, 15);

	// Top face
	pTriList[8].set(16, 17, 18);
	pTriList[9].set(16, 18, 19);

	// Bottom face
	pTriList[10].set(20, 21, 22);
	pTriList[11].set(20, 22, 23);
}


void ModelTools::CreateUnitSphere(int slices, int stacks, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = (slices + 1) * (stacks + 1);
	ntri = slices * stacks * 2;
	pVerts = new StandardVertex[nverts];
	pTriList = new TriangleByIndex[ntri];

	int vertIndex = 0;
	int triIndex = 0;

	for (int stack = 0; stack <= stacks; ++stack) {
		float phi = stack * 3.14f / stacks;
		float y = cos(phi);
		float radius = sin(phi);

		for (int slice = 0; slice <= slices; ++slice) {
			float theta = slice * 2.0f * 3.14f / slices;
			float x = radius * cos(theta);
			float z = radius * sin(theta);

			float u = static_cast<float>(slice) / slices;
			float v = static_cast<float>(stack) / stacks;

			// Directly use the position as the normal
			pVerts[vertIndex++].set(x, y, z, u, v, x, y, z);
		}
	}

	for (int stack = 0; stack < stacks; ++stack) {
		for (int slice = 0; slice < slices; ++slice) {
			int first = stack * (slices + 1) + slice;
			int second = first + slices + 1;

			// Corrected winding order
			pTriList[triIndex++].set(first, first + 1, second);
			pTriList[triIndex++].set(second, first + 1, second + 1);
		}
	}
}

