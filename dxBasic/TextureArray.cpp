/*
================================
 texture.cpp

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "texturearray.h"

/*
================
 Texture::Texture
================
*/
TextureArray::TextureArray(void)
	
{
	textures_[0] = NULL;
	textures_[1] = NULL;
	textures_[2] = NULL;
}

/*
================
 Texture::~Texture
================
*/
TextureArray::~TextureArray(void)
{

}

/*
================
 Texture::init
================
*/
bool TextureArray::init( ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3 )
{
	HRESULT result;
	
	result = D3DX11CreateShaderResourceViewFromFileA(device, filename1.c_str(), NULL, NULL, &textures_[0], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFileA(device, filename2.c_str(), NULL, NULL, &textures_[1], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFileA(device, filename3.c_str(), NULL, NULL, &textures_[2], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*
================
 Texture::shutdown
================
*/
void TextureArray::shutdown()
{
	if (textures_[0])
	{

		textures_[0]->Release();
		textures_[0] = NULL;
	}

	if (textures_[1])
	{

		textures_[1]->Release();
		textures_[1] = NULL;
	}

	if (textures_[1])
	{

		textures_[1]->Release();
		textures_[1] = NULL;
	}
}

/*
================
 Texture::getTexture
================
*/
ID3D11ShaderResourceView** TextureArray::getTextureArray()
{
	return textures_;
}
