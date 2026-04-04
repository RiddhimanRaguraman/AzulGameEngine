#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

SkyBox::SkyBox(ID3D11Device* dev, float len, float urep, float vrep)
{
    int nverts = 24;
    StandardVertex *pVerts = new StandardVertex[nverts];

    int ntri = 12;
    TriangleByIndex *pTriList = new TriangleByIndex[ntri];

    float half = 0.5f * len;

    // Define normals for each face
    Vect up(0, 1, 0);
    Vect down(0, -1, 0);
    Vect left(-1, 0, 0);
    Vect right(1, 0, 0);
    Vect front(0, 0, 1);
    Vect back(0, 0, -1);

    // Default color (white)
    Vect color(1, 1, 1);

    // Define cube vertices (each face has unique vertices)
    // Bottom face
    pVerts[0].set(-half, -half, -half, 0.25f * urep,  1.0f, down, color);
    pVerts[1].set(-half, -half,  half,  0.5f * urep,  1.0f, down, color);
    pVerts[2].set( half, -half,  half,  0.5f * urep, 0.65f, down, color);
    pVerts[3].set( half, -half, -half, 0.25f * urep, 0.65f, down, color);

    // Top face
    pVerts[4].set(-half, half, -half,  0.5f * urep,    0.0f * vrep, up, color);
    pVerts[5].set(-half, half,  half,  0.5f * urep, 0.3334f * vrep, up, color);
    pVerts[6].set( half, half,  half, 0.25f * urep, 0.3334f * vrep, up, color);
    pVerts[7].set( half, half, -half, 0.25f * urep,    0.0f * vrep, up, color);

    // Left face
    pVerts[8].set (-half, -half, -half, 0.75f * urep,   0.65f * vrep, left, color);
    pVerts[9].set (-half,  half, -half, 0.75f * urep, 0.3334f * vrep, left, color);
    pVerts[10].set(-half,  half,  half,  0.5f * urep, 0.3334f * vrep, left, color);
    pVerts[11].set(-half, -half,  half,  0.5f * urep,   0.65f * vrep, left, color);

    // Right face
    pVerts[12].set(half, -half, -half,  0.0f * urep,   0.65f * vrep, right, color);
    pVerts[13].set(half,  half, -half,  0.0f * urep, 0.3334f * vrep, right, color);
    pVerts[14].set(half,  half,  half, 0.25f * urep, 0.3334f * vrep, right, color);
    pVerts[15].set(half, -half,  half, 0.25f * urep,   0.65f * vrep, right, color);

    // Front face
    pVerts[16].set(-half, -half, half,  0.5f * urep,   0.65f * vrep, front, color);
    pVerts[17].set(-half,  half, half,  0.5f * urep, 0.3334f * vrep, front, color);
    pVerts[18].set( half,  half, half, 0.25f * urep, 0.3334f * vrep, front, color);
    pVerts[19].set( half, -half, half, 0.25f * urep,   0.65f * vrep, front, color);

    // Back face
    pVerts[20].set(-half, -half, -half, 0.75f * urep,   0.65f * vrep, back, color);
    pVerts[21].set(-half,  half, -half, 0.75f * urep, 0.3334f * vrep, back, color);
    pVerts[22].set( half,  half, -half,  1.0f * urep, 0.3334f * vrep, back, color);
    pVerts[23].set( half, -half, -half,  1.0f * urep,   0.65f * vrep, back, color);

    // Define cube triangles (2 per face, 6 faces total)
    pTriList[0].set(0, 1, 2);
    pTriList[1].set(0, 2, 3);  // Bottom face

    pTriList[2].set(4, 6, 5);
    pTriList[3].set(4, 7, 6);  // Top face

    pTriList[4].set(8, 9, 10);
    pTriList[5].set(8, 10, 11); // Left face

    pTriList[6].set(12, 15, 14);
    pTriList[7].set(12, 14, 13); // Right face

    pTriList[8].set(16, 17, 18);
    pTriList[9].set(16, 18, 19); // Front face

    pTriList[10].set(20, 23, 22);
    pTriList[11].set(20, 22, 21); // Back face

    // Create the model
    pSkyBox = new Model(dev, pVerts, nverts, pTriList, ntri);

    // Clean up allocated memory
    delete[] pVerts;
    delete[] pTriList;
}

SkyBox::~SkyBox()
{
    delete pSkyBox;
}

Model* SkyBox::GetModel()
{
    return pSkyBox;
}


void SkyBox::Render(ID3D11DeviceContext* context)
{
    pSkyBox->SetToContext(context);
    pSkyBox->Render(context);
}
