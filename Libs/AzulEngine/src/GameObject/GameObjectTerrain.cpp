#include "GameObjectTerrain.h"

#include "MathEngine.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "TexNodeMan.h"
#include "RenderComponent.h"

namespace Azul
{
	GameObjectTerrain::GameObjectTerrain(Mesh::Name mesh,
		ShaderObject::Name shader,
		TextureObject::Name tex,
		float _authoredSizeX, float _authoredSizeZ, float _authoredHeight)
		: GameObjectRigidBody(MaterialKind::FlatTexture),
		authoredSizeX(_authoredSizeX),
		authoredSizeZ(_authoredSizeZ),
		authoredHeight(_authoredHeight),
		sizeX(_authoredSizeX),
		sizeZ(_authoredSizeZ),
		height(_authoredHeight),
		uRepeat(1.0f),
		vRepeat(1.0f)
	{
		assert(this->authoredSizeX > 0.0f);
		assert(this->authoredSizeZ > 0.0f);
		assert(this->authoredHeight > 0.0f);

		// Fill the render handles (data-path -- no GraphicsObject).
		RenderComponent& r = this->GetRender();
		r.pMesh = MeshNodeMan::Find(mesh);
		r.pShader = ShaderObjectNodeMan::Find(shader);
		r.pTex = TexNodeMan::Find(tex);
		assert(r.pMesh);
		assert(r.pShader);
		assert(r.pTex);

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

		// (was GraphicsObject_FlatTexture::SetUVRepeat) -> RenderComponent.uvMatrix
		Scale s(this->uRepeat, this->vRepeat, 1.0f);
		this->GetRender().uvMatrix = s;
	}

	void GameObjectTerrain::privApplyScale()
	{
		const float sx = this->sizeX / this->authoredSizeX;
		const float sy = this->height / this->authoredHeight;
		const float sz = this->sizeZ / this->authoredSizeZ;

		this->SetScale(sx, sy, sz);
	}
}
