//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "File.h"
#include "textureDataConverter.h"
#include "vboDataConverter.h"
#include "MeshLayout.h"

namespace Azul
{
	char* GLTF::ExportJSON(const char* const pFileName)
	{
		assert(pFileName);
		char sBuff[40];
		sprintf_s(sBuff, 40, "JSON(%s) \n", pFileName);
		Trace::out("\n");
		Trace::out("%-25s --> %s\n", "export", sBuff);

		bool status;
		tinygltf::Model gltfModel;

		// Load the gltfModel
		status = GLTF::Load(gltfModel, pFileName);
		assert(status);

		// Read glb into memory (raw buffer)
		char* poBuff = nullptr;
		unsigned int BuffSize(0);

		status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
		assert(status);

		// Get GLB_Header
		GLB_header glbHeader;
		status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
		assert(status);

		// Get Raw JSON
		char* poJSON = nullptr;
		unsigned int JsonSize(0);
		status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
		assert(status);


		FILE* fh;
		system("del *.json");
		fopen_s(&fh, "Gltf.json", "wt");
		fwrite(poJSON, JsonSize, 1, fh);
		fclose(fh);

		delete poBuff;

		return poJSON;
	}
	bool GLTF::OutputQuat(Model &model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex)
	{
		Trace::out("Quat bone:%d Frame:%d \n", NodeIndex, FrameIndex);
		unsigned char *pBuff = (unsigned char *)&model.buffers[0].data[0];
		auto QuatAAccessor = model.accessors[AccessorIndex];
		auto QuatABuffView = model.bufferViews[(size_t)QuatAAccessor.bufferView];

		pBuff = pBuff + QuatABuffView.byteOffset;
		assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
		Vec4f *pVect4 = (Vec4f *)pBuff;
		for(size_t i = 0; i < QuatAAccessor.count; i++)
		{
			if(FrameIndex == i)
			{
				Trace::out("\t Quat(%ff,%ff,%ff,%ff);\n",
						   pVect4->x, pVect4->y, pVect4->z, pVect4->w);
			}
			pVect4++;
		}

		Trace::out("\n");

		return true;
	}


	bool GLTF::OutputTrans(Model &model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex)
	{
		Trace::out("Trans bone:%d Frame:%d \n", NodeIndex, FrameIndex);
		unsigned char *pBuff = (unsigned char *)&model.buffers[0].data[0];
		auto TransAccessor = model.accessors[AccessorIndex];
		auto TransBuffView = model.bufferViews[(size_t)TransAccessor.bufferView];

		pBuff = pBuff + TransBuffView.byteOffset;
		assert(TransAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(TransAccessor.type == TINYGLTF_TYPE_VEC3);
		Vec3f *pVect3 = (Vec3f *)pBuff;
		for(size_t i = 0; i < TransAccessor.count; i++)
		{
			if(FrameIndex == i)
			{
				Trace::out("\t Vec3(%ff,%ff,%ff);  \n",
						   pVect3->x, pVect3->y, pVect3->z);
			}
			pVect3++;
		}
		Trace::out("\n");

		return true;
	}

	bool GLTF::Load(Model& model, const char* const pFileName)
	{
		TinyGLTF loader;
		std::string err;
		std::string warn;

		bool status = loader.LoadBinaryFromFile(&model, &err, &warn, pFileName);

		if (!warn.empty())
		{
			Trace::out("Warn: %s\n", warn.c_str());
			assert(false);
		}

		if (!err.empty())
		{
			Trace::out("Err: %s\n", err.c_str());
			assert(false);
		}

		if (!status)
		{
			Trace::out2("Failed to parse glTF\n");
			assert(false);
		}

		return status;
	}

	bool GLTF::GetGLBRawBuffer(char*& pBuff, unsigned int& BuffSize, const char* const pFileName)
	{
		unsigned char* poNewBuff;
		unsigned int newBuffSize;

		File::Error err;
		err = File::GetFileAsBuffer(pFileName, poNewBuff, newBuffSize);
		assert(err == File::Error::SUCCESS);

		pBuff = (char*)poNewBuff;
		BuffSize = newBuffSize;

		assert(pBuff);
		assert(BuffSize > 0);

		return true;
	}

	bool GLTF::GetGLBHeader(GLB_header& header, const char* const pBuff, unsigned int BuffSize)
	{
		GLB_header* pGlbHeader;

		pGlbHeader = (GLB_header*)&pBuff[0];
		assert(pGlbHeader);

		// boundary check
		assert((char*)(pGlbHeader + 1) < (pBuff + BuffSize));

		assert(pGlbHeader->version == 2);
		assert(pGlbHeader->magic == 0x46546C67);

		header = *pGlbHeader;

		return true;
	}

	bool GLTF::GetRawJSON(char*& pJSON, unsigned int& JsonSize, const char* const pBuff, unsigned int BuffSize)
	{
		// BEGINNING...
		GLB_header* pGlbHeader;

		pGlbHeader = (GLB_header*)&pBuff[0];
		assert(pGlbHeader);

		// boundary check
		assert((char*)(pGlbHeader + 1) < (pBuff + BuffSize));

		// Now this next is CHUNK header
		CHUNK_header* pChunkHdr;
		pChunkHdr = (CHUNK_header*)(pGlbHeader + 1);

		assert(pChunkHdr->chunkType == 0x4E4F534A);

		JsonSize = pChunkHdr->chunkLength;
		assert(JsonSize > 0);

		// boundary check - bottom of header
		assert((char*)(pChunkHdr + 1) < (pBuff + BuffSize));

		// boundary check - bottom of data
		assert(((char*)(pChunkHdr + 1) + pChunkHdr->chunkLength) < (pBuff + BuffSize));

		// 0 zero the string first
		size_t JsonLen = JsonSize;
		pJSON = new char[JsonLen]();

		assert(pJSON);
		memset(pJSON, 0, JsonLen);
		memcpy_s(pJSON, JsonLen, pChunkHdr->chunkData, JsonSize);

		FILE* fh;
		fopen_s(&fh, "Gltf.json", "w");
		fwrite(pJSON, JsonLen, 1, fh);
		fclose(fh);

		return true;
	}

	bool GLTF::GetBinaryBuffPtr(char*& pBinaryBuff, unsigned int& BinaryBuffSize, const char* const pBuff, unsigned int BuffSize)
	{
		// BEGINNING...
		GLB_header* pGlbHeader;

		pGlbHeader = (GLB_header*)&pBuff[0];
		assert(pGlbHeader);

		// Now this next is CHUNK header - JSON
		CHUNK_header* pChunkHdr;
		pChunkHdr = (CHUNK_header*)(pGlbHeader + 1);

		assert(pChunkHdr);
		assert(pChunkHdr->chunkType == 0x4E4F534A);

		//Trace::out("CHUNK Header:\n");
		//Trace::out("\tType: 0x%08x \n", pChunkHdr->chunkType);
		//Trace::out("\tLength: 0x%x %d\n", pChunkHdr->chunkLength, pChunkHdr->chunkLength);

		// Now this next is CHUNK header - Binary
		pChunkHdr = (CHUNK_header*)((size_t)(pChunkHdr + 1) + pChunkHdr->chunkLength);

		assert(pChunkHdr->chunkType == 0x004E4942);

		//Trace::out("CHUNK Header:\n");
		//Trace::out("\tType: 0x%08x \n", pChunkHdr->chunkType);
		//Trace::out("\tLength: 0x%x %d\n", pChunkHdr->chunkLength, pChunkHdr->chunkLength);

		BinaryBuffSize = pChunkHdr->chunkLength;
		pBinaryBuff = (char*)pChunkHdr->chunkData;

		assert(((char*)(pChunkHdr + 1) + BinaryBuffSize) <= (pBuff + BuffSize));

		assert(pBinaryBuff);
		assert(BinaryBuffSize > 0);

		return true;

	}

	bool GLTF::GetAzulProtoType(const char*& pProtoFileType, textureData& _textData)
	{
		bool status = false;
		if (_textData.enabled)
		{
			pProtoFileType = ".t";
			status = true;
		}
		else
		{
			assert(false);
			status = false;
		}

		return status;
	}

	bool GLTF::GetAzulProtoType(const char*& pProtoFileType, meshData& _meshData)
	{
		bool status = false;

		if (_meshData.vbo_vert.enabled)
		{
			pProtoFileType = ".m";
			status = true;
		}
		else
		{
			assert(false);
			status = false;
		}

		return status;
	}

	bool GLTF::SetVBO(Model& gltfModel, const char* pKey, vboData& vbo, char* pBinaryBuff, int idx)
	{
		// Get the accessor, buffer view
		vbo.enabled = false;

		assert(pKey);
		auto map = gltfModel.meshes[idx].primitives[0].attributes;
		auto it = map.find(pKey);

		if (it != map.end())
		{

			vbo.enabled = true;

			auto index = it->second;

			// Get the accessor, buffer view
			auto accessor = gltfModel.accessors[(size_t)index];
			size_t buffIndex = (size_t)accessor.bufferView;
			auto bufferView = gltfModel.bufferViews[buffIndex];

			vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
			vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
			vbo.vboType = vboDataConverter::GetType(accessor.type);

			// Use bufferView.byteStride if provided, otherwise compute from type/component
			unsigned int computedStride = GetStride(vbo.vboType, vbo.componentType);
			unsigned int byteStride = (bufferView.byteStride > 0) ? (unsigned int)bufferView.byteStride : computedStride;
			vbo.stride = byteStride;
			vbo.count = (uint32_t)accessor.count;
			// Data size should be based on accessor range, not entire bufferView
			vbo.dataSize = vbo.count * vbo.stride;

			// poData
			assert(pBinaryBuff);
			char* pBuffStart(nullptr);

			// Start address must include both bufferView and accessor offsets
			pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;

			// in case there's data
			delete[] vbo.poData;

			vbo.poData = new unsigned char[vbo.dataSize]();
			assert(vbo.poData);
			memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);

			// Positions and normals are preserved as-authored; no TRS/normalization.
		}

		return vbo.enabled;
	}

	bool GLTF::SetVBO_index(Model& gltfModel, const char* pKey, vboData& vbo, char* pBinaryBuff, int idx)
	{
		AZUL_UNUSED_VAR(pKey);

		vbo.enabled = true;

		auto index = gltfModel.meshes[idx].primitives[0].indices;
		assert(index >= 0);

		// Get the accessor, buffer view
		auto accessor = gltfModel.accessors[(size_t)index];
		size_t buffIndex = (size_t)accessor.bufferView;
		auto bufferView = gltfModel.bufferViews[buffIndex];
		vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
		vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
		vbo.vboType = vboDataConverter::GetType(accessor.type);
		vbo.count = (uint32_t)accessor.count;
		// For indices, data size should be count * sizeof(component)
		unsigned int indexStride = 0;
		if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_SHORT)
		{
			indexStride = sizeof(unsigned short);
		}
		else if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT)
		{
			indexStride = sizeof(unsigned int);
		}
		else if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_BYTE)
		{
			indexStride = sizeof(unsigned char);
		}
		else
		{
			// Unsupported index type
			assert(false);
		}
		vbo.dataSize = vbo.count * indexStride;

		// poData
		assert(pBinaryBuff);
		char* pBuffStart(nullptr);
		// Start address must include both bufferView and accessor offsets
		pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;
		// in case there's data
		delete[] vbo.poData;
		if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_SHORT)
		{
			// Convert the unsigned short into unsigned int buffer
			// Engine will now be all unsigned int for index buffer
			unsigned int* pIndexData = new unsigned int[vbo.count]();
			unsigned int* pTmp = pIndexData;
			unsigned short* pShort = (unsigned short*)pBuffStart;
			for (unsigned int i = 0; i < vbo.count; i++)
			{
				// convert it
				*pTmp++ = *pShort++;
			}
			// this is the output buffer
			vbo.poData = (unsigned char*)pIndexData;
			// update the data
			vbo.dataSize = vbo.count * sizeof(unsigned int);
			// update the component
			vbo.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;
		}
		else if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT)
		{
			vbo.poData = new unsigned char[vbo.dataSize]();
			assert(vbo.poData);
			memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);
		}
		else
		{
			assert(false);
		}
		return true;
	}

	bool GLTF::SetTexture(Model& gltfModel, const char* pKey, textureData& text, char* pBinaryBuff, int idx)
	{
		AZUL_UNUSED_VAR(pKey);

		text.enabled = false;

		// Resolve texture -> image from material
		int texIndex = gltfModel.materials[idx].pbrMetallicRoughness.baseColorTexture.index;
		if (texIndex < 0 || texIndex >= (int)gltfModel.textures.size())
		{
			return false;
		}
		const Texture& tex = gltfModel.textures[texIndex];
		int imageIndex = tex.source;
		if (imageIndex < 0 || imageIndex >= (int)gltfModel.images.size())
		{
			return false;
		}
		const Image& img = gltfModel.images[imageIndex];

		text.enabled = true;

		// Mime -> texture type (avoid incorrect asserts)
		if (strcmp(img.mimeType.c_str(), "image/png") == 0)
		{
			text.textType = textureData::TEXTURE_TYPE::PNG;
		}
		else
		{
			text.textType = textureData::TEXTURE_TYPE::TGA; // placeholder for non-png
		}

		text.magFilter = textureData::TEXTURE_MAG_FILTER::DEFAULT;
		text.minFilter = textureData::TEXTURE_MIN_FILTER::DEFAULT;
		text.wrapS = textureData::TEXTURE_WRAP::DEFAULT;
		text.wrapT = textureData::TEXTURE_WRAP::DEFAULT;
		text.width = (unsigned int)img.width;
		text.height = (unsigned int)img.height;
		text.component = (unsigned int)img.component;
		text.bits = (unsigned int)img.bits;
		text.as_is = img.as_is;
		text.pixel_type = textureDataConverter::GetComponent(img.pixel_type);

		// pFileName
		unsigned int len = (uint32_t)img.name.length();
		assert(len < text.FILE_NAME_SIZE);
		memcpy_s(text.pFileName, text.FILE_NAME_SIZE, img.name.c_str(), len);

		if (img.as_is == false)
		{
			// decompressed bytes already in memory
			text.dataSize = (uint32_t)img.image.size();
			delete[] text.poData;
			text.poData = new unsigned char[text.dataSize]();
			assert(text.poData);
			auto pStart = &img.image[0];
			memcpy_s(text.poData, text.dataSize, pStart, text.dataSize);
		}
		else
		{
			// compressed buffer via bufferView
			unsigned int buffViewIndex = (unsigned int)img.bufferView;
			auto buffView = gltfModel.bufferViews[buffViewIndex];
			text.dataSize = (uint32_t)buffView.byteLength;

			char* pBuffStart = pBinaryBuff + buffView.byteOffset;
			delete[] text.poData;
			text.poData = new unsigned char[text.dataSize]();
			assert(text.poData);
			memcpy_s(text.poData, text.dataSize, pBuffStart, text.dataSize);
		}

		return text.enabled;
	}

	// Robust GLB texture extraction: uses texture->source to find image
	bool GLTF::SetTextureGLB(Model& gltfModel, const char* pKey, textureData& text, char* pBinaryBuff, int idx)
	{
		AZUL_UNUSED_VAR(pKey);

		text.enabled = false;

		// Material texture index points into textures array
		int texIndex = gltfModel.materials[idx].pbrMetallicRoughness.baseColorTexture.index;
		if (texIndex < 0 || texIndex >= (int)gltfModel.textures.size())
		{
			return false;
		}

		const Texture& tex = gltfModel.textures[texIndex];
		int imageIndex = tex.source;
		if (imageIndex < 0 || imageIndex >= (int)gltfModel.images.size())
		{
			return false;
		}

		const Image& img = gltfModel.images[imageIndex];

		text.enabled = true;

		// Mime -> texture type
		if (strcmp(img.mimeType.c_str(), "image/png") == 0)
		{
			text.textType = textureData::TEXTURE_TYPE::PNG;
		}
		else if (strcmp(img.mimeType.c_str(), "image/jpeg") == 0 || strcmp(img.mimeType.c_str(), "image/jpg") == 0)
		{
			text.textType = textureData::TEXTURE_TYPE::JPEG;
		}
		else
		{
			// Default fallback
			text.textType = textureData::TEXTURE_TYPE::PNG;
		}

		text.magFilter = textureData::TEXTURE_MAG_FILTER::DEFAULT;
		text.minFilter = textureData::TEXTURE_MIN_FILTER::DEFAULT;
		text.wrapS = textureData::TEXTURE_WRAP::DEFAULT;
		text.wrapT = textureData::TEXTURE_WRAP::DEFAULT;
		text.width = (unsigned int)img.width;
		text.height = (unsigned int)img.height;
		text.component = (unsigned int)img.component;
		text.bits = (unsigned int)img.bits;
		text.as_is = img.as_is;
		text.pixel_type = textureDataConverter::GetComponent(img.pixel_type);

		// Set reasonable format/component enums now; CreateTexture may refine later
		if (img.component == 4)
		{
			text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
			text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
		}
		else if (img.component == 3)
		{
			text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGB;
			text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
		}
		else
		{
			text.eFormat = textureData::TEXTURE_EFORMAT::DEFAULT;
			text.nComponent = textureData::TEXTURE_NCOMPONENT::DEFAULT;
		}

		// pFileName
		unsigned int len = (uint32_t)img.name.length();
		if (len >= text.FILE_NAME_SIZE)
		{
			len = text.FILE_NAME_SIZE - 1;
		}
		memset(text.pFileName, 0, text.FILE_NAME_SIZE);
		memcpy_s(text.pFileName, text.FILE_NAME_SIZE, img.name.c_str(), len);

		if (img.as_is == false)
		{
			// Decompressed raw buffer is available
			text.dataSize = (uint32_t)img.image.size();
			delete[] text.poData;
			text.poData = new unsigned char[text.dataSize]();
			assert(text.poData);
			auto pStart = &img.image[0];
			memcpy_s(text.poData, text.dataSize, pStart, text.dataSize);
		}
		else
		{
			// Export the texture file in compressed mode via bufferView
			unsigned int buffViewIndex = (unsigned int)img.bufferView;
			auto buffView = gltfModel.bufferViews[buffViewIndex];
			text.dataSize = (uint32_t)buffView.byteLength;

			char* pBuffStart = pBinaryBuff + buffView.byteOffset;

			delete[] text.poData;
			text.poData = new unsigned char[text.dataSize]();
			assert(text.poData);
			memcpy_s(text.poData, text.dataSize, pBuffStart, text.dataSize);
		}

		return text.enabled;
	}

	bool GLTF::SetCustomVBO(vboData& vbo,
		void* pData,
		unsigned int sizeInBytes,
		unsigned int count,
		vboData::VBO_COMPONENT componentType,
		vboData::VBO_TYPE type,
		vboData::VBO_TARGET target)
	{
		vbo.enabled = true;

		vbo.targetType = target;
		vbo.componentType = componentType;
		vbo.vboType = type;
		vbo.stride = GetStride(vbo.vboType, vbo.componentType);
		vbo.count = count;
		vbo.dataSize = sizeInBytes;

		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pData, vbo.dataSize);

		return true;
	}


	bool GLTF::SetCustomVBO_index(vboData& vbo,
		void* pData,
		unsigned int sizeInBytes,
		unsigned int count,
		vboData::VBO_COMPONENT componentType,
		vboData::VBO_TYPE type,
		vboData::VBO_TARGET target)
	{
		vbo.enabled = true;

		vbo.targetType = target;
		vbo.componentType = componentType;
		vbo.vboType = type;
		vbo.stride = GetStride(vbo.vboType, vbo.componentType);
		vbo.count = count;
		vbo.dataSize = sizeInBytes;

		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pData, vbo.dataSize);

		return true;
	}


	unsigned int GLTF::GetStride(vboData::VBO_TYPE type, vboData::VBO_COMPONENT comp)
	{
		unsigned int count = 0;
		unsigned int size = 0;

		switch (type)
		{
		case vboData::VBO_TYPE::SCALAR:
			count = 1;
			break;

		case vboData::VBO_TYPE::VEC2:
			count = 2;
			break;

		case vboData::VBO_TYPE::VEC3:
			count = 3;
			break;

		case vboData::VBO_TYPE::VEC4:
			count = 4;
			break;

		case vboData::VBO_TYPE::MAT2:
			count = 4;
			break;

		case vboData::VBO_TYPE::MAT3:
			count = 9;
			break;

		case vboData::VBO_TYPE::MAT4:
			count = 16;
			break;

		case vboData::VBO_TYPE::VECTOR:
		case vboData::VBO_TYPE::MATRIX:
		default:
			assert(false);
			count = 0;
			break;
		}

		switch (comp)
		{
		case vboData::VBO_COMPONENT::BYTE:
			size = 1;
			break;

		case vboData::VBO_COMPONENT::UNSIGNED_BYTE:
			size = 1;
			break;

		case vboData::VBO_COMPONENT::SHORT:
			size = 2;
			break;

		case vboData::VBO_COMPONENT::UNSIGNED_SHORT:
			size = 2;
			break;

		case vboData::VBO_COMPONENT::INT:
			size = 4;
			break;

		case vboData::VBO_COMPONENT::UNSIGNED_INT:
			size = 4;
			break;

		case vboData::VBO_COMPONENT::FLOAT:
			size = 4;
			break;

		case vboData::VBO_COMPONENT::DOUBLE:
			size = 8;
			break;

		default:
			assert(false);
			size = 0;
			break;
		}

		return count * size;
	}


}

// --- End of File ---
