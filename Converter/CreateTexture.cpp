//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CreateTexture.h"
#include "textureData.h"
#include "GLTF.h"
#include "textureDataConverter.h"
#include "File.h"
#include "stb_image.h"

using namespace Azul;
using namespace tinygltf;

namespace Azul
{
	void gltReadTGABits(const char* pFileName, textureData& textData);
	void gltReadPNGBits(const char* pFileName, textureData& textData);

	// Define targa header. This is only used locally.
#pragma pack(1)
	typedef struct
	{
		char	identsize;               // Size of ID field that follows header (0)
		char	colorMapType;            // 0 = None, 1 = paletted
		char	imageType;               // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
		unsigned short	colorMapStart;   // First colour map entry
		unsigned short	colorMapLength;  // Number of colors
		unsigned char 	colorMapBits;    // bits per palette entry
		unsigned short	xstart;          // image x origin
		unsigned short	ystart;          // image y origin
		unsigned short	width;           // width in pixels
		unsigned short	height;          // height in pixels
		char	bits;                    // bits per pixel (8 16, 24, 32)
		char	descriptor;              // image descriptor
	} TGAHEADER;
#pragma pack(8)


	void CreateTexturePNG(const char* const pFileName, const char* const pTargetName)
	{
		assert(pFileName);

		Trace::out("%-25s", pFileName);

		// Strip the extension .tga off the name
		unsigned int len = (uint32_t)strlen(pFileName);
		char* pTmp = new char[len + 1]();
		memset(pTmp, 0, len);
		memcpy(pTmp, pFileName, len - strlen(".png"));

		// base name
		std::string BaseName = pTargetName;
		delete[] pTmp;

		// runtime model
		textureData  Mesh;

		//---------------------------------
		// Model Name
		//---------------------------------
		Mesh.enabled = true;
		Mesh.textType = textureData::TEXTURE_TYPE::PNG;
		Mesh.magFilter = textureData::TEXTURE_MAG_FILTER::MAG_LINEAR;
		Mesh.minFilter = textureData::TEXTURE_MIN_FILTER::MIN_LINEAR;
		Mesh.wrapS = textureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
		Mesh.wrapT = textureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
		Mesh.pixel_type = textureData::TEXTURE_COMPONENT::UNSIGNED_BYTE;
		Mesh.as_is = false;

		memcpy_s(Mesh.pFileName, textureData::FILE_NAME_SIZE, pFileName, strlen(pFileName));

		gltReadPNGBits(pFileName, Mesh);

		textureData_proto mA_proto;
		Mesh.Serialize(mA_proto);

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;
		bool status;

		// Create output name
		const char* pProtoFileType = nullptr;
		status = GLTF::GetAzulProtoType(pProtoFileType, Mesh);
		assert(status);

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";

		Trace::out(" --> %+30s\n", OutputFileName.c_str());

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), (DWORD)strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

	}

	void CreateTextureTGA(const char* const pFileName, const char* const pTargetName)
	{
		assert(pFileName);

		Trace::out("%-25s", pFileName);

		// Strip the extension .tga off the name
		unsigned int len = (uint32_t)strlen(pFileName);
		char* pTmp = new char[len + 1]();
		memset(pTmp, 0, len);
		memcpy(pTmp, pFileName, len - strlen(".tga"));

		// base name
		std::string BaseName = pTargetName;
		delete[] pTmp;

		// runtime model
		textureData  Mesh;

		//---------------------------------
		// Model Name
		//---------------------------------

		Mesh.enabled = true;
		Mesh.textType = textureData::TEXTURE_TYPE::TGA;
		Mesh.magFilter = textureData::TEXTURE_MAG_FILTER::MAG_LINEAR;
		Mesh.minFilter = textureData::TEXTURE_MIN_FILTER::MIN_LINEAR;
		Mesh.wrapS = textureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
		Mesh.wrapT = textureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
		Mesh.pixel_type = textureData::TEXTURE_COMPONENT::UNSIGNED_BYTE;
		Mesh.as_is = false;

		memcpy_s(Mesh.pFileName, textureData::FILE_NAME_SIZE, pFileName, strlen(pFileName));


		gltReadTGABits(pFileName, Mesh);

		textureData_proto mA_proto;
		Mesh.Serialize(mA_proto);

		// -------------------------------
		//  Write to file
		//--------------------------------

		File::Handle fh;
		File::Error err;
		bool status;

		// Create output name
		const char* pProtoFileType = nullptr;
		status = GLTF::GetAzulProtoType(pProtoFileType, Mesh);
		assert(status);

		std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";

		Trace::out(" --> %22s\n", OutputFileName.c_str());

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), (DWORD)strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

	}

	//----------------------------------------------------------------------------
	// Convert embedded GLB texture to .t.proto.azul
	//----------------------------------------------------------------------------
	void ConvertTextureGLB(const char* const pGLBFileName, const char* const pTargetName)
	{
		assert(pGLBFileName);
		assert(pTargetName);

		Trace::out("%-25s", pGLBFileName);

		// Base name used for output proto file(s)
		std::string BaseName = pTargetName;

		// Load GLB model
		Model gltfModel;
		bool ok = GLTF::Load(gltfModel, pGLBFileName);
		assert(ok);

		// Obtain raw GLB buffer and binary chunk pointer (for compressed image data)
		char* pRawBuff = nullptr;
		unsigned int rawSize = 0;
		ok = GLTF::GetGLBRawBuffer(pRawBuff, rawSize, pGLBFileName);
		assert(ok);

		char* pBinaryBuff = nullptr;
		unsigned int binSize = 0;
		ok = GLTF::GetBinaryBuffPtr(pBinaryBuff, binSize, pRawBuff, rawSize);
		assert(ok);

		// Export textures per mesh primitive index to align with mesh files
		const int meshCount = (int)gltfModel.meshes.size();
		const bool singleOutput = (meshCount <= 1);

		for (int meshIdx = 0; meshIdx < meshCount; ++meshIdx)
		{
			textureData Text{};
			Text.enabled = false;
			Text.magFilter = textureData::TEXTURE_MAG_FILTER::DEFAULT;
			Text.minFilter = textureData::TEXTURE_MIN_FILTER::DEFAULT;
			Text.wrapS = textureData::TEXTURE_WRAP::DEFAULT;
			Text.wrapT = textureData::TEXTURE_WRAP::DEFAULT;
			Text.pixel_type = textureData::TEXTURE_COMPONENT::DEFAULT;
			Text.as_is = false;

			// Resolve material index for this mesh primitive
			int matIndex = -1;
			if (!gltfModel.meshes[meshIdx].primitives.empty())
			{
				matIndex = gltfModel.meshes[meshIdx].primitives[0].material;
			}

			if (matIndex < 0 || matIndex >= (int)gltfModel.materials.size())
			{
				// Fallback: if no material, try first image directly
				if (gltfModel.images.empty())
				{
					continue;
				}
				// Build a minimal textureData from first image
				int imageIndex = 0;
				const Image& img = gltfModel.images[imageIndex];
				Text.enabled = true;
				Text.textType = (strcmp(img.mimeType.c_str(), "image/png") == 0)
					? textureData::TEXTURE_TYPE::PNG
					: textureData::TEXTURE_TYPE::JPEG;
				Text.width = (unsigned int)img.width;
				Text.height = (unsigned int)img.height;
				Text.component = (unsigned int)img.component;
				Text.bits = (unsigned int)img.bits;
				Text.as_is = img.as_is;
				Text.pixel_type = textureDataConverter::GetComponent(img.pixel_type);

				unsigned int len = (uint32_t)img.name.length();
				if (len >= Text.FILE_NAME_SIZE) len = Text.FILE_NAME_SIZE - 1;
				memset(Text.pFileName, 0, Text.FILE_NAME_SIZE);
				memcpy_s(Text.pFileName, Text.FILE_NAME_SIZE, img.name.c_str(), len);

				if (img.as_is == false)
				{
					Text.dataSize = (uint32_t)img.image.size();
					delete[] Text.poData;
					Text.poData = new unsigned char[Text.dataSize]();
					assert(Text.poData);
					auto pStart = &img.image[0];
					memcpy_s(Text.poData, Text.dataSize, pStart, Text.dataSize);
				}
				else
				{
					unsigned int buffViewIndex = (unsigned int)img.bufferView;
					auto buffView = gltfModel.bufferViews[buffViewIndex];
					Text.dataSize = (uint32_t)buffView.byteLength;
					char* pBuffStart = pBinaryBuff + buffView.byteOffset;
					delete[] Text.poData;
					Text.poData = new unsigned char[Text.dataSize]();
					assert(Text.poData);
					memcpy_s(Text.poData, Text.dataSize, pBuffStart, Text.dataSize);
				}
			}
			else
			{
				ok = GLTF::SetTextureGLB(gltfModel, "baseColorTexture", Text, pBinaryBuff, matIndex);
				if (!ok || !Text.enabled)
				{
					continue;
				}
			}
			if (!ok || !Text.enabled)
			{
				continue;
			}

			// Ensure engine-friendly 4-channel uncompressed data and component enums
			if (Text.as_is)
			{
				// Compressed path: decode to RGBA using stb_image
				int outW = 0, outH = 0, outComp = 0;

				int texIndex = gltfModel.materials[matIndex].pbrMetallicRoughness.baseColorTexture.index;
				int imageIndex = gltfModel.textures[texIndex].source;
				unsigned int buffViewIndex = (unsigned int)gltfModel.images[imageIndex].bufferView;
				auto buffView = gltfModel.bufferViews[buffViewIndex];
				unsigned char* pStart = (unsigned char*)(pBinaryBuff + buffView.byteOffset);
				int srcSize = (int)buffView.byteLength;

				unsigned char* decoded = stbi_load_from_memory(pStart, srcSize, &outW, &outH, &outComp, 4);
				assert(decoded);

				// Update textureData to decompressed 4-channel
				Text.width = (unsigned int)outW;
				Text.height = (unsigned int)outH;
				Text.component = 4;
				Text.bits = 32;
				Text.dataSize = (unsigned int)(outW * outH * 4);
				delete[] Text.poData;
				Text.poData = decoded;
				Text.as_is = false;

				// Set format/component enums based on channels
				Text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
				Text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
			}
			else
			{
				// Decompressed path from tinygltf: may be 3 or 4 channels; ensure 4-channel
				if (Text.component == 3)
				{
					unsigned int numTexels = Text.width * Text.height;
					unsigned int newSize = numTexels * 4U;

					unsigned char* pNew = new unsigned char[newSize]();
					unsigned int j = 0;
					unsigned int i = 0;
					for (unsigned int k = 0; k < numTexels; ++k)
					{
						pNew[j] = Text.poData[i];
						pNew[j + 1] = Text.poData[i + 1];
						pNew[j + 2] = Text.poData[i + 2];
						pNew[j + 3] = 0xFF;
						j += 4;
						i += 3;
					}

					delete[] Text.poData;
					Text.poData = pNew;
					Text.dataSize = newSize;
					Text.component = 4;
					Text.bits = 32;

					Text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGB;
					Text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
				}
				else if (Text.component == 4)
				{
					Text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
					Text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
				}
				else
				{
					// Unexpected, default to RGBA assumptions
					Text.component = 4;
					Text.eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
					Text.nComponent = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
				}
			}

			// Choose proto type and output name (single vs multi-material)
			const char* pProtoFileType = nullptr;
			ok = GLTF::GetAzulProtoType(pProtoFileType, Text);
			assert(ok);

			std::string OutputFileName;
			if (singleOutput)
			{
				OutputFileName = BaseName + pProtoFileType + ".proto.azul";
			}
			else
			{
				OutputFileName = BaseName + "_m" + std::to_string(meshIdx) + pProtoFileType + ".proto.azul";
			}
			Trace::out(" --> %s\n", OutputFileName.c_str());

			// Serialize and write
			textureData_proto t_proto;
			Text.Serialize(t_proto);

			File::Handle fh;
			File::Error err;
			err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
			assert(err == File::Error::SUCCESS);

			std::string strOut;
			t_proto.SerializeToString(&strOut);

			File::Write(fh, strOut.data(), (uint32_t)strOut.length());
			assert(err == File::Error::SUCCESS);

			err = File::Close(fh);
			assert(err == File::Error::SUCCESS);
		}

		// Clean up raw buffer
		delete[] pRawBuff;
	}

	//-----------------------------------------------------------------------------
	// Allocate memory and load targa bits. Returns pointer to new buffer,
	// height, and width of texture, and the OpenGL format of data.
	// Call free() on buffer when finished!
	// This only works on pretty vanilla targas... 8, 24, or 32 bit color
	// only, no palettes, no RLE encoding.
	//-----------------------------------------------------------------------------
	void gltReadTGABits(const char* pFileName, textureData& textData)
	{
		TGAHEADER tgaHeader;        // TGA file header
		unsigned long lImageSize;   // Size in bytes of image
		short sDepth;               // Pixel depth;
		char* pBits = nullptr;      // Pointer to bits

		// Default/Failed values
		unsigned int Width = 0;
		unsigned int Height = 0;
		textureData::TEXTURE_EFORMAT eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGB;
		textureData::TEXTURE_NCOMPONENT nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;

		// Attempt to open the file
		File::Handle fh;
		File::Error err;

		err = File::Open(fh, pFileName, File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, &tgaHeader, sizeof(TGAHEADER));
		assert(err == File::Error::SUCCESS);

		Width = tgaHeader.width;
		Height = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		tgaHeader.ystart = tgaHeader.height;

		if (sDepth == 3)
		{
			// Crap... DirectX loading issue... promote 3 bytes to 4 bytes
			unsigned int numTexels = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height;
			unsigned int oldSize = numTexels * (unsigned int)sDepth;

			unsigned char* pBuff = new unsigned char[oldSize]();
			assert(pBuff);

			err = File::Read(fh, pBuff, oldSize);
			assert(err == File::Error::SUCCESS);

			err = File::Close(fh);
			assert(err == File::Error::SUCCESS);

			lImageSize = numTexels * 4;
			unsigned char* pNewBuff = new unsigned char[lImageSize]();;

			unsigned int j(0);
			unsigned int i(0);
			for (unsigned int k = 0; k < numTexels; k++)
			{

				// w1(4)
				pNewBuff[j] = pBuff[i];
				pNewBuff[j + 1] = pBuff[i + 1];
				pNewBuff[j + 2] = pBuff[i + 2];
				pNewBuff[j + 3] = 0xFF;

				j = j + 4;
				i = i + 3;
			}

			delete[] pBuff;

			sDepth = 4;
			tgaHeader.bits = 32;
			pBits = (char*)pNewBuff;

			// w1(4)
			eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_BGR;
			nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGR;
		}
		else
		{
			// Put some validity checks here. Very simply, I only understand
			// or care about 8, 24, or 32 bit targa's.
			if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
			{
				assert(false);
			}

			// Calculate size of image buffer
			lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

			pBits = new char[lImageSize * sizeof(char)]();
			assert(pBits);

			err = File::Read(fh, pBits, lImageSize);
			assert(err == File::Error::SUCCESS);

			err = File::Close(fh);
			assert(err == File::Error::SUCCESS);

			// w1(4)
			eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_BGRA;
			nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGRA;
		}

		textData.eFormat = eFormat;
		textData.nComponent = nComponents;
		textData.width = Width;
		textData.height = Height;
		textData.component = (unsigned int)sDepth;
		textData.bits = (unsigned int)tgaHeader.bits;
		textData.dataSize = lImageSize;
		textData.poData = (unsigned char*)pBits;

	}

	void gltReadPNGBits(const char* pFileName, textureData& textData)
	{

		unsigned long ImageSize;   // Size in bytes of image
		unsigned char* pBits = nullptr;      // Pointer to bits

		// Default/Failed values
		int Width = 0;
		int Height = 0;
		int comp;
		int req_comp = 4;

		// some default value
		textureData::TEXTURE_EFORMAT eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGB;
		textureData::TEXTURE_NCOMPONENT nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;

		pBits = stbi_load(pFileName, &Width, &Height, &comp, req_comp);

		ImageSize = (unsigned int)(Width * Height * req_comp);

		if (comp == 3)
		{
			// w1(4)
			eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGB;
			nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
		}
		else if (comp == 4)
		{
			// w1(4)
			eFormat = textureData::TEXTURE_EFORMAT::EFORMAT_RGBA;
			nComponents = textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
		}
		else
		{
			assert(false);
		}

		textData.eFormat = eFormat;
		textData.nComponent = nComponents;
		textData.width = (unsigned int)Width;
		textData.height = (unsigned int)Height;
		textData.component = (unsigned int)req_comp;
		textData.bits = (unsigned int)comp;
		textData.dataSize = ImageSize;
		textData.poData = (unsigned char*)pBits;

	}
}
// --- End of File ---
