/*
================================
 d3dmanager.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <string>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>


/*
================================
 
 D3DManager

 A wrapper class for Direct3D initialization.
 Creates the device sets up swap chain and the back buffer.
 
================================
*/
class D3DManager
{
public:
	D3DManager(void);
	D3DManager(const D3DManager&);
	~D3DManager(void);

	bool init( int screenWidth, int screenHeight, const bool vsync, HWND hwnd, const bool fullscreen, const float screenDepth, const float screenNear );
	void shutdown();
	
	void beginScene( float r, float g, float b, float a );
	void endScene();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void getProjectionMatrix(D3DXMATRIX&);
	void getWorldMatrix(D3DXMATRIX&);
	void getOrthoMatrix(D3DXMATRIX&);

	void getGpuInfo(char*, int&); // FIXME: change to string
	ID3D11DepthStencilView* getDepthStencilView();

	void turnZBufferOn();
	void turnZBufferOff();

	void setBackBufferRenderTarget();
	void resetViewport();

private:
	bool vsyncEnabled_;
	int gpuMemory_;
	char gpuDescription_[128];    // FIXME
	IDXGISwapChain* swapChain_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	ID3D11RenderTargetView* renderTargetView_;
	ID3D11Texture2D* depthStencilBuffer_;
	ID3D11DepthStencilState* depthStencilState_;
	ID3D11DepthStencilView* depthStencilView_;
	ID3D11RasterizerState* rasterizerState_;
	D3DXMATRIX projectionMatrix_;
	D3DXMATRIX worldMatrix_;
	D3DXMATRIX orthoMatrix_;
	ID3D11DepthStencilState* depthDisableStencilState_;
	D3D11_VIEWPORT viewport_; // can be made non-member?
};

