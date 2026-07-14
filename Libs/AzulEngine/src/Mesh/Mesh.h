#ifndef MESH_H
#define MESH_H
#include "EngineDLLInterface.h"

#include <d3d11.h>
#include "DLink.h"
#include "MathEngine.h"
#include "Camera.h"
#include "BufferCBV_vs.h"

namespace Azul
{

	// Quick Hack
	struct AZUL_ENGINE_LIBRARY_API Vec2f
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

	struct AZUL_ENGINE_LIBRARY_API Vec3f
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

	struct AZUL_ENGINE_LIBRARY_API Vec3ui
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
	struct AZUL_ENGINE_LIBRARY_API VertexColor
	{
		Vec3f Color;
	};

	struct AZUL_ENGINE_LIBRARY_API VertexPos
	{
		Vec3f Position;
	};

	struct AZUL_ENGINE_LIBRARY_API VertexNorm
	{
		Vec3f Norm;
	};

	struct AZUL_ENGINE_LIBRARY_API VertexTexCoord
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


	class AZUL_ENGINE_LIBRARY_API Mesh
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
		Ward,
		R2D2,
		BONE_A,
		BONE_B,
		ChickenBot,
		Pirate,
		Mousey,
		Drax,
		Maw,
		Halo0,
		Halo1,
		Crownboi0,
		Crownboi1,
		Crownboi2,
		Crownboi3,
		Crownboi4,
		Crownboi5,
		Crownboi6,
		Crownboi7,
		Crownboi8,
		Crownboi9,
		Crownboi10,
		Crownboi11,
		Crownboi12,
		Crownboi13,
		Crownboi14,
		Crownboi15,
		Crownboi16,
		Crownboi17,
		Crownboi18,
		Crownboi19,
		Crownboi20,
		Crownboi21,
		Crownboi22,
		Crownboi23,
		Terrain,
		SkyBox,
		CUBE_COLOR,
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
		virtual void RenderIndexBufferInstanced(unsigned int instanceCount);
		// Fill the per-vertex color stream client-side (for meshes loaded without
		// one, e.g. the proto cube used with ColorByVertex). numBytes/stride are the
		// color buffer's size and per-vertex stride.
		virtual void SetVertexColors(const void *pColorData, unsigned int numBytes, unsigned int stride);
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
