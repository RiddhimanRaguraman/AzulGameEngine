#ifndef _TerrainModel
#define _TerrainModel

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{

private:
	DirectX::ScratchImage scrtTex;

	Model* pModTerrain;
	int terrainWidth;
	int terrainHeight;
	float cellSizeX;
	float cellSizeZ;

	float maxHeight;
	float yTrans;

public:
	TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);
	~TerrainModel();

	float ComputeHeightAt(int col, int row);

	Model* GetModel();
	void Render(ID3D11DeviceContext* context);

};



#endif _TerrainModel