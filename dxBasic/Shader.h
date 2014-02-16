/*
================================
 shader.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include <vector>


/*
================================
 
 Shader

 The default shader class. The corresponding light.fx shader
 is used for rendering moveable objects. This class allows choosing which
 technique within the shader is used for rendering.
 
================================
*/
class Shader
{
public:
	Shader(void);	
	~Shader(void);
		
	bool init(ID3D11Device* device, HWND hwnd);
	void shutdown();
	void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, std::vector<ID3D11ShaderResourceView*>, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, float);
		
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_samplerState;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
	
	bool initShader(ID3D11Device*, HWND, WCHAR*, WCHAR* );
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR* );
	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, std::vector<ID3D11ShaderResourceView*>, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, float);
	void renderShader(ID3D11DeviceContext*, int);

	struct MatrixBuffer
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBuffer
	{
		D3DXVECTOR3 cameraPostion;
		float padding;
	};
	

	struct LightBuffer
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float padding1;
		D3DXVECTOR4 specularColor;
		float specularPower;
		float specularIntensity;
		float padding2;
		float padding3;
	};
};

