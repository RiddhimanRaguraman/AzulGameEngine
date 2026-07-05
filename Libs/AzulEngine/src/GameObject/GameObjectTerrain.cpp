#include "GameObjectTerrain.h"

#include "GraphicsObject_FlatTexture.h"

namespace Azul
{
	GameObjectTerrain::GameObjectTerrain(GraphicsObject_FlatTexture* pGraphicsObject, float _authoredSizeX, float _authoredSizeZ, float _authoredHeight)
		: GameObjectRigidBody(pGraphicsObject),
		pGraphicsObjectFlat(pGraphicsObject),
		authoredSizeX(_authoredSizeX),
		authoredSizeZ(_authoredSizeZ),
		authoredHeight(_authoredHeight),
		sizeX(_authoredSizeX),
		sizeZ(_authoredSizeZ),
		height(_authoredHeight),
		uRepeat(1.0f),
		vRepeat(1.0f)
	{
		assert(this->pGraphicsObjectFlat);
		assert(this->authoredSizeX > 0.0f);
		assert(this->authoredSizeZ > 0.0f);
		assert(this->authoredHeight > 0.0f);

		this->privApplyScale();
		this->SetUVRepeat(1.0f, 1.0f);
	}

	void GameObjectTerrain::SetSize(float _sizeX, float _sizeZ)
	{
		assert(_sizeX > 0.0f);
		assert(_sizeZ > 0.0f);

		this->sizeX = _sizeX;
		this->sizeZ = _sizeZ;
		this->privApplyScale();
	}

	void GameObjectTerrain::SetHeight(float _height)
	{
		assert(_height > 0.0f);

		this->height = _height;
		this->privApplyScale();
	}

	void GameObjectTerrain::SetUVRepeat(float _uRepeat, float _vRepeat)
	{
		assert(_uRepeat > 0.0f);
		assert(_vRepeat > 0.0f);

		this->uRepeat = _uRepeat;
		this->vRepeat = _vRepeat;

		this->pGraphicsObjectFlat->SetUVRepeat(this->uRepeat, this->vRepeat);
	}

	void GameObjectTerrain::privApplyScale()
	{
		const float sx = this->sizeX / this->authoredSizeX;
		const float sy = this->height / this->authoredHeight;
		const float sz = this->sizeZ / this->authoredSizeZ;

		this->SetScale(sx, sy, sz);
	}
}

