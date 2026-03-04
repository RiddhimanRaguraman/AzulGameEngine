//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <Windows.h>
#include <string>
#include <d3d11.h>

#include "File.h"
#include "TextureObject.h"
#include "Engine.h"
#include "StringThis.h"
#include "StateDirectXMan.h"

namespace Azul
{
	TextureObject::TextureObject(const char *const pMeshFileName,
								 D3D11_FILTER filter)
		: mName(TextureObject::Name::Uninitialized),
		SRVTexBuffer(),
		SamplerStateBuffer(),
		Texture2DBuffer(),
		width(0),
		height(0),
		AlphaEnabled(false)
	{

		unsigned char *poBuff;
		unsigned int numBytes;

		File::Error ferror;
		ferror = File::GetFileAsBuffer(pMeshFileName, poBuff, numBytes);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		std::string strIn((const char *)poBuff, numBytes);
		textureData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		textureData mB;
		mB.Deserialize(mB_proto);

		delete[] poBuff;

		// Do I know this format?
		assert(strcmp(mB.pVersion, "2.0.0") == 0);

		//----------------------

		// Create texture
		this->width = mB.width;;
		this->height = mB.height;;

		assert((mB.textType == textureData::TEXTURE_TYPE::PNG)
			   || (mB.textType == textureData::TEXTURE_TYPE::TGA));


		this->Texture2DBuffer.Initialize(mB.width,
										 mB.height,
										 mB.nComponent,
										 mB.poData);

		this->AlphaEnabled = Texture2DBuffer.GetAlphaMode();


		this->SRVTexBuffer.Initialize(mB.nComponent, Texture2DBuffer);


		this->SamplerStateBuffer.Initialize(filter);

	}

	bool TextureObject::HasAlpha() const
	{
		return this->AlphaEnabled;
	}

	TextureObject::~TextureObject()
	{

	}

	void TextureObject::ActivateTexture()
	{
		SRVTexBuffer.SetActive(ShaderResourceBufferSlot::TexA);
		SamplerStateBuffer.SetActive(ShaderSamplerSlot::SampleA);
	}

	void TextureObject::SetName(TextureObject::Name _name)
	{
		this->mName = _name;
	}

	char *TextureObject::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->mName));
		return pTmp;
	}

}

// --- End of File ---