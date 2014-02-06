/*
================================
 shader.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>


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
	void render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, float);
		
private:
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* layout_;
	ID3D11Buffer* matrixBuffer_;
	ID3D11SamplerState* samplerState_;
	ID3D11Buffer* cameraBuffer_;
	ID3D11Buffer* lightBuffer_;
	
	//ID3D10Effect* effect_;
	//ID3D10EffectTechnique* technique_;
	//ID3D10InputLayout* layout_;
	//ID3D10EffectMatrixVariable* worldMatrixPtr_;
	//ID3D10EffectMatrixVariable* viewMatrixPtr_;
	//ID3D10EffectMatrixVariable* projectionMatrixPtr_;
	//ID3D10EffectShaderResourceVariable* texturePrt_;
	//ID3D10EffectVectorVariable* lightDirectionPtr_;
	//ID3D10EffectVectorVariable* ambientColorPtr_;
	//ID3D10EffectVectorVariable* diffuseColorPtr_;
	//ID3D10EffectVectorVariable* cameraPositionPtr_;
	//ID3D10EffectVectorVariable* specularColorPtr_;
	//ID3D10EffectScalarVariable* specularPowerPtr_;
	//ID3D10EffectScalarVariable* specularIntensityPtr_;

	
	bool initShader(ID3D11Device*, HWND, WCHAR*, WCHAR* );
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR* );
	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR3, D3DXVECTOR4, float, float);
	void renderShader(ID3D11DeviceContext*, int);

	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPostion;
		float padding;
	};
	

	struct LightBufferType
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

