//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include "DLink.h"
#include "MathEngine.h"
#include "Camera.h"
#include "BufferCBV_vs.h"

namespace Azul
{

	// Quick Hack
	struct Vec2f
	{
		Vec2f()
			: u(0.0f), v(0.0f)
		{
		}
		Vec2f(const Vec2f &) = default;
		Vec2f &operator = (const Vec2f &) = default;
		~Vec2f() = default;

		Vec2f(const float _u, const float _v)
			: u(_u), v(_v)
		{
		}

		float u;
		float v;
	};

	struct Vec3f
	{
		Vec3f()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}
		Vec3f(const Vec3f &) = default;
		Vec3f &operator = (const Vec3f &) = default;
		~Vec3f() = default;

		Vec3f(const float a, const float b, const float c)
			: x(a), y(b), z(c)
		{
		}

		void set(const Vec3f &r)
		{
			this->x = r.x;
			this->y = r.y;
			this->z = r.z;
		}

		float len()
		{
			return sqrtf(x * x + y * y + z * z);
		}

		Vec3f operator+( const Vec3f &r)
		{
			Vec3f v;

			v.x = this->x + r.x;
			v.y = this->y + r.y;
			v.z = this->z + r.z;

			return v;
		}

		float dot(const Vec3f &r)
		{
			return x * r.x + y * r.y + z * r.z;
		}

		Vec3f operator * (float s)
		{
			Vec3f v;

			v.x = this->x * s;
			v.y = this->y * s;
			v.z = this->z * s;

			return v;
		}

		Vec3f &operator += (const Vec3f &r)
		{
			this->x += r.x;
			this->y += r.y;
			this->z += r.z;

			return *this;
		}

		Vec3f operator - (const Vec3f &r)
		{
			Vec3f v;

			v.x = this->x - r.x;
			v.y = this->y - r.y;
			v.z = this->z - r.z;

			return v;
		}

		float x;
		float y;
		float z;
	};
	struct Vec4f  // Skin
	{
		Vec4f()
			: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
		{
		}
		Vec4f(const Vec4f&) = default;
		Vec4f& operator = (const Vec4f&) = default;
		~Vec4f() = default;

		Vec4f(const float a, const float b, const float c, const float d)
			: x(a), y(b), z(c), w(d)
		{
		}


		float x;
		float y;
		float z;
		float w;
	};

	struct Vec3ui
	{
		Vec3ui() = default;
		Vec3ui(const Vec3ui&) = default;
		Vec3ui& operator = (const Vec3ui&) = default;
		~Vec3ui() = default;

		Vec3ui(const unsigned int a, const unsigned int b, const unsigned int c)
			: x(a), y(b), z(c)
		{
		}


		unsigned int x;
		unsigned int y;
		unsigned int z;
	};

	struct Vec4ui  // Skin
	{
		Vec4ui() = default;
		Vec4ui(const Vec4ui&) = default;
		Vec4ui& operator = (const Vec4ui&) = default;
		~Vec4ui() = default;

		Vec4ui(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d)
			: x(a), y(b), z(c), w(d)
		{
		}

		unsigned int x;
		unsigned int y;
		unsigned int z;
		unsigned int w;
	};

	struct Vec4si  // Skin
	{
		Vec4si() = default;
		Vec4si(const Vec4si&) = default;
		Vec4si& operator = (const Vec4si&) = default;
		~Vec4si() = default;

		Vec4si(const unsigned short a, const unsigned short b, const unsigned short c, const unsigned short d)
			: x(a), y(b), z(c), w(d)
		{
		}

		unsigned short x;
		unsigned short y;
		unsigned short z;
		unsigned short w;
	};


	// Vertex data for a colored cube.
	struct VertexColor
	{
		Vec3f Color;
	};

	struct VertexPos
	{
		Vec3f Position;
	};

	struct VertexNorm
	{
		Vec3f Norm;
	};

	struct VertexTexCoord
	{
		Vec2f TexCoord;
	};
	struct VertexWeights   // Skin
	{
		Vec4f Weight;
	};

	struct VertexJoints    // Skin
	{
		Vec4ui Joint;
	};


	class Mesh
	{
	public:
		const static unsigned int MESH_MODEL_NAME_SIZE = 128;
		enum class Name
		{
		NULL_MESH,
			CUBE,
			PYRAMID,
		CRATE,
		SPHERE,
		SPRITE,
		SPACE_FRIGATE,
		R2D2,
		BONE_A,
		BONE_B,
		ChickenBot,
		Mousey,
		ChickenBot_Bone,
		ChickenBot_Bone_001,
		ChickenBot_Bone_L,
		ChickenBot_Bone_L_001,
		ChickenBot_Bone_L_002,
		ChickenBot_Bone_R,
		ChickenBot_Bone_R_001,
		ChickenBot_Bone_R_002,
		DogBot_BackFoot_L,
		DogBot_BackFoot_R,
		DogBot_BackLowLeg_L,
		DogBot_BackLowLeg_R,
		DogBot_BackUpLeg_L,
		DogBot_BackUpLeg_R,
		DogBot_FrontFoot_L,
		DogBot_FrontFoot_R,
		DogBot_FrontLowLeg_L,
		DogBot_FrontLowLeg_R,
		DogBot_FrontUpLeg_L,
		DogBot_FrontUpLeg_R,
		DogBot_gun_L,
		DogBot_gun_R,
		DogBot_spine,

		SpiderBot_Bone,
		SpiderBot_Bone_L_001,
		SpiderBot_Bone_L_002,
		SpiderBot_Bone_L_003,
		SpiderBot_Bone_L_004,
		SpiderBot_Bone_L_005,
		SpiderBot_Bone_L_006,
		SpiderBot_Bone_L_007,
		SpiderBot_Bone_L,
		SpiderBot_Bone_R_001,
		SpiderBot_Bone_R_002,
		SpiderBot_Bone_R_003,
		SpiderBot_Bone_R_004,
		SpiderBot_Bone_R_005,
		SpiderBot_Bone_R_006,
		SpiderBot_Bone_R_007,
		SpiderBot_Bone_R,
		NOT_INITIALIZED
		};

	public:
		Mesh() = delete;
		Mesh(Mesh &) = delete;
		Mesh &operator = (Mesh &) = delete;
		virtual ~Mesh();

		Mesh(unsigned int numVerts, unsigned int numTriangles);

		void Wash();
		bool Compare(DLink *pTarget);
		char *GetName();
		void SetName(Mesh::Name _name);

		virtual void ActivateMesh() = 0;
		virtual void RenderIndexBuffer() = 0;
		// optional override
		virtual void ActivateSRVBuffers();
		virtual void ActivateConstantBuffers();
		virtual void TransferConstantBuffers();
		virtual void Initialize_SkinBoneWorldBuffer(size_t _structSize);
		virtual void Transfer_SkinBoneWorldBuffer(Mat4* p);

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		Name name;
		char pModelName[MESH_MODEL_NAME_SIZE];
		unsigned int numVerts;
		unsigned int numIndices;

	};
}

#endif

// --- End of File ---
