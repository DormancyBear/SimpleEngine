#include <memory>
#include <DirectXTex.h>
#include "textureclass.h"
#include <Graphics/DirectXPlatformManager.h>

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(WCHAR const * filename)
{
	wchar_t ext[_MAX_EXT];
	_wsplitpath_s(filename, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	DirectX::ScratchImage image;
	HRESULT hr;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = LoadFromTGAFile(filename, nullptr, image);
	}
	else if (_wcsicmp(ext, L".hdr") == 0)
	{
		hr = LoadFromHDRFile(filename, nullptr, image);
	}
	// .bmp, .jpg, .png, etc.
	else
	{
		hr = LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, nullptr, image);
	}
	if (FAILED(hr))
	{
		return false;
	}

	hr = CreateShaderResourceView(DirectXPlatformManager::Instance().GetDevice(),
		image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &m_texture);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}