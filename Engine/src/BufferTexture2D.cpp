//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "BufferTexture2D.h"
#include "StateDirectXMan.h"

namespace Azul
{

	BufferTexture2D::BufferTexture2D()
		: mWidth{0},
		mHeight{0},
		mFormat{DXGI_FORMAT_UNKNOWN},
		pData{nullptr},
		bAlphaEnabled{false},
		poTex{nullptr},
		bCreate{false}
	{
	}

	BufferTexture2D::BufferTexture2D(ID3D11Texture2D *p)
		: mWidth{0},
		mHeight{0},
		mFormat{DXGI_FORMAT_UNKNOWN},
		pData{nullptr},
		bAlphaEnabled{false},
		poTex{nullptr},
		bCreate{false}
	{
		assert(p);
		this->privInitialize(p);
	}

	void BufferTexture2D::privInitialize(ID3D11Texture2D *p)
	{
		assert(p);
		this->poTex = p;

		D3D11_TEXTURE2D_DESC Desc;
		this->poTex->GetDesc(&Desc);

		this->mFormat = Desc.Format;
		this->mWidth = Desc.Width;
		this->mHeight = Desc.Height;
		this->pData = nullptr;
		this->bCreate = true;
	}

	void BufferTexture2D::Initialize(unsigned int Width,
									 unsigned int Height,
									 textureData::TEXTURE_NCOMPONENT Component,
									 unsigned char *_pData)
	{
		mWidth = Width;
		mHeight = Height;
		assert(mFormat == DXGI_FORMAT_UNKNOWN);
		pData = _pData;
		assert(bAlphaEnabled == false);
		assert(poTex == nullptr);
		assert(bCreate == false);
	
		if(Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGRA
		   || Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA)
		{
			this->bAlphaEnabled = true;
		}
		else
		{
			this->bAlphaEnabled = false;
		}

		if(Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB
		   || Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA)
		{
			mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		if(Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGR
		   || Component == textureData::TEXTURE_NCOMPONENT::NCOMPONENT_BGRA)
		{
			mFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
		}

		assert(mFormat != DXGI_FORMAT_UNKNOWN);
		assert(pData);
		assert(mWidth >= 1);
		assert(mHeight >= 1);

		this->privCreate();
	}

	void BufferTexture2D::privCreate()
	{
		// in case set was called before
		assert(this->bCreate == false);
		this->bCreate = true;

		// Create texture
		HRESULT hr;

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = mFormat;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pData;
		subResource.SysMemPitch = mWidth * 4;
		subResource.SysMemSlicePitch = 0;

		hr = StateDirectXMan::GetDevice()->CreateTexture2D(&desc, &subResource, &poTex);
		assert(SUCCEEDED(hr));
		assert(this->poTex);
	}

	ID3D11Texture2D *BufferTexture2D::GetID3D11Texture2D()
	{
		assert(this->bCreate == true);
		assert(this->poTex);

		return this->poTex;
	}

	bool BufferTexture2D::GetAlphaMode()
	{
		assert(this->bCreate == true);
		return this->bAlphaEnabled;
	}

	BufferTexture2D::~BufferTexture2D()
	{
		SafeRelease(this->poTex);
	}

}

//--- End of File ---
