#ifndef GAME_OBJECT_TERRAIN_H
#define GAME_OBJECT_TERRAIN_H

#include "GameObjectRigidBody.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"

#include "EngineDLLInterface.h"

namespace Azul
{
	// Data-path FlatTexture terrain: no GraphicsObject. Its render handles live in
	// the entity's RenderComponent (filled in the ctor); SetUVRepeat writes the
	// RenderComponent.uvMatrix (the "late setter" that made a snapshot impossible).
	class AZUL_ENGINE_LIBRARY_API GameObjectTerrain : public GameObjectRigidBody
	{
	public:
		GameObjectTerrain(Mesh::Name mesh,
			ShaderObject::Name shader,
			TextureObject::Name tex,
			float authoredSizeX, float authoredSizeZ, float authoredHeight);
		GameObjectTerrain() = delete;
		GameObjectTerrain(const GameObjectTerrain&) = delete;
		GameObjectTerrain& operator=(const GameObjectTerrain&) = delete;
		virtual ~GameObjectTerrain() = default;

		void SetSize(float sizeX, float sizeZ);
		void SetHeight(float height);
		void SetUVRepeat(float uRepeat, float vRepeat);

	private:
		void privApplyScale();

	private:
		float authoredSizeX;
		float authoredSizeZ;
		float authoredHeight;

		float sizeX;
		float sizeZ;
		float height;

		float uRepeat;
		float vRepeat;
	};
}

#endif
