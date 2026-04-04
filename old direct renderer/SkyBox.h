// SkyBox.h
// Berthiaume Jan 2017

#ifndef _SkyBox
#define _SkyBox

#include "Matrix.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
private:
	Model* pSkyBox;

public:
	SkyBox(const SkyBox&) = delete; 
	SkyBox(Model&&) = delete; 
	SkyBox& operator=(const SkyBox&) & = delete;
	SkyBox& operator=(SkyBox&&) & = delete; 
	~SkyBox();

	Model* GetModel();

	SkyBox(ID3D11Device* dev, float len, float urep, float vrep);

	void Render(ID3D11DeviceContext* context);
};

#endif _SkyBox

