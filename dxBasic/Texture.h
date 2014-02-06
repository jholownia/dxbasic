/*
================================
 texture.h

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <D3D11.h>
#include <D3DX11.h>
/*
================================
 
 Texture

 A simple wrapper for texture.
 
================================
*/
class Texture
{
public:
	Texture(void);
	~Texture(void);

	bool init(ID3D11Device* device, WCHAR* filename);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();

private:
	ID3D11ShaderResourceView* texture_;
};

