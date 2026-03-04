//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_H
#define GLTF_H

#include "tiny_gltf.h"
using namespace tinygltf;

#include "meshData.h"
#include "MeshLayout.h"
#include "MathEngine.h"

namespace Azul
{
	struct GLB_header
	{
		uint32_t magic;
		uint32_t version;
		uint32_t length;
	};

	struct CHUNK_header
	{
		uint32_t chunkLength;
		uint32_t chunkType;
		unsigned char chunkData[];
	};

	struct skin_weight_joint_index
	{
		skin_weight_joint_index() = default;
		~skin_weight_joint_index() = default;
		size_t	index;
		Vec4f	skinWeight;
		Vec4si	jointIndex;
	};

	struct SkinMesh
	{
		std::string skinName;
		size_t      vertCount;
		size_t      polyCount;

		std::vector< int > jointTable;

		// Skin data...
		std::vector< Mat4 > invBone;
		std::vector< Vec4ui > joints;  // orig unsigned short--> unsigned int
		std::vector< Vec4f > weights;

		// Mesh data...
		std::vector< Vec3f > pos;
		std::vector< Vec3f > norms;
		std::vector< Vec2f > text_coord;
		std::vector< Vec4f > color;  // rgba

		// Trilist indexing
		std::vector< Vec3ui > index;  // orig unsigned short--> unsigned int
	};


	class GLTF
	{
	public:


		static bool OutputTrans(Model &model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex);
		static char* ExportJSON(const char* const pFileName);
		static bool OutputQuat(Model &model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex);


		static bool Load(Model& model, const char* const pFileName);

		static bool GetGLBRawBuffer(char*& pBuff, unsigned int& BuffSize, const char* const pFileName);
		static bool GetGLBHeader(GLB_header& header, const char* const pBuff, unsigned int BuffSize);
		static bool GetRawJSON(char*& pJSON, unsigned int& JsonSize, const char* const pBuff, unsigned int BuffSize);
		static bool GetBinaryBuffPtr(char*& pBinaryBuff, unsigned int& BinaryBuffSize, const char* const pBuff, unsigned int BuffSize);

		static bool SetVBO(Model& gltfModel, const char* pKey, vboData& vbo, char* pBinaryBuff, int idx);
		static bool SetVBO_index(Model& gltfModel, const char* pKey, vboData& vbo, char* pBinaryBuff, int idx);
		static bool SetTexture(Model& gltfModel, const char* pKey, textureData& text, char* pBinaryBuff, int idx);
		// GLB-specific texture extractor that maps texture index to image source
		static bool SetTextureGLB(Model& gltfModel, const char* pKey, textureData& text, char* pBinaryBuff, int idx);

		static bool GetAzulProtoType(const char*& pProtoFileType, meshData& _meshData);
		static bool GetAzulProtoType(const char*& pProtoFileType, textureData& _textData);



		static bool SetCustomVBO(vboData& vbo,
			void* data,
			unsigned int sizeInBytes,
			unsigned int count,
			vboData::VBO_COMPONENT componentType,
			vboData::VBO_TYPE type,
			vboData::VBO_TARGET target);

		static bool SetCustomVBO_index(vboData& vbo,
			void* data,
			unsigned int sizeInBytes,
			unsigned int count,
			vboData::VBO_COMPONENT componentType,
			vboData::VBO_TYPE type,
			vboData::VBO_TARGET target);

		static unsigned int GetStride(vboData::VBO_TYPE type, vboData::VBO_COMPONENT comp);

	};
}
#endif

// --- End of File ---
