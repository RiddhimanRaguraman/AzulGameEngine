#ifndef GAME_OBJECT_TERRAIN_H
#define GAME_OBJECT_TERRAIN_H

#include "GameObjectRigidBody.h"

namespace Azul
{
	class GraphicsObject_FlatTexture;

	class GameObjectTerrain : public GameObjectRigidBody
	{
	public:
		GameObjectTerrain(GraphicsObject_FlatTexture* pGraphicsObject, float authoredSizeX, float authoredSizeZ, float authoredHeight);
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
		GraphicsObject_FlatTexture* pGraphicsObjectFlat;

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

