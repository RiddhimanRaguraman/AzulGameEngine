#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>
#include <vector>

TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
    : maxHeight(maxheight), yTrans(ytrans)
{
    HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
    assert(SUCCEEDED(hr));

    const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
    assert(hgtmap->height == hgtmap->width);

    size_t side = hgtmap->height;
    size_t pixel_width = 4;
    float cellSize = len / (side - 1);

    // Create vertices
    int nverts = side * side;
    StandardVertex* pVerts = new StandardVertex[nverts];

    terrainWidth = hgtmap->width;
    terrainHeight = hgtmap->height;
    cellSizeX = len / (terrainWidth - 1);
    cellSizeZ = len / (terrainHeight - 1);
    nverts = terrainWidth * terrainHeight;

    for (size_t i = 0; i < terrainHeight; i++)
    {
        for (size_t j = 0; j < terrainWidth; j++)
        {
            int index = i * terrainWidth + j;
            float xPos = -0.5f * len + j * cellSizeX;
            float zPos = -0.5f * len + i * cellSizeZ;
            float heightValue = ComputeHeightAt(j, i);

            // Calculate normals using central difference method
            Vect normal;
            float hL = ComputeHeightAt(j - 1, i);
            float hR = ComputeHeightAt(j + 1, i);
            float hD = ComputeHeightAt(j, i - 1);
            float hU = ComputeHeightAt(j, i + 1);

            normal.set(hL - hR, 2.0f, hD - hU);
            normal.norm();

            pVerts[index].set(
                xPos, heightValue, zPos,
                (float)j / (terrainWidth - 1) * RepeatU,
                (float)i / (terrainHeight - 1) * RepeatV,
                normal,
                Vect(1, 1, 1)
            );
        }
    }

    int ntri = (side - 1) * (side - 1) * 2;
    TriangleByIndex* pTriList = new TriangleByIndex[ntri];

    int triIndex = 0;
    for (size_t i = 0; i < side - 1; i++)
    {
        for (size_t j = 0; j < side - 1; j++)
        {
            int index = i * side + j;
            pTriList[triIndex++].set(index, index + side, index + 1);
            pTriList[triIndex++].set(index + 1, index + side, index + side + 1);
        }
    }

    // Create the Model object
    pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);

    // Stop Memory Leak
    delete[] pVerts;
    delete[] pTriList;
}
float TerrainModel::ComputeHeightAt(int col, int row)
{
    // Ensure indices are within valid bounds
    if (col < 0 || col >= terrainWidth || row < 0 || row >= terrainHeight)
        return 0.0f; // Return default height for out-of-bounds

    const DirectX::Image* img = scrtTex.GetImage(0, 0, 0);
    if (!img || !img->pixels)
        return 0.0f;

    DXGI_FORMAT format = scrtTex.GetMetadata().format;
    size_t pixelWidth = (format == DXGI_FORMAT_R8_UNORM) ? 1 : 4;
    uint8_t* pixel = img->pixels + (row * img->rowPitch) + (col * pixelWidth);

    uint8_t heightValue;
    if (pixelWidth == 1) {
        heightValue = pixel[0]; // Grayscale
    }
    else {
        heightValue = pixel[0]; // Red channel from RGBA
    }

    return (heightValue / 255.0f) * maxHeight + yTrans; // Normalized height
}
Model* TerrainModel::GetModel()
{
    return pModTerrain;
}
TerrainModel::~TerrainModel()
{
    delete pModTerrain;
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
    pModTerrain->SetToContext(context);
    pModTerrain->Render(context);
}