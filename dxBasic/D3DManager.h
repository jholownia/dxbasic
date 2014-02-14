/*
================================
 d3dmanager.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : C4005 ) // Get rid of annoying warnings related to using DirecX SDK instead of Windows 8 SDK

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
 Creates the device, device context, sets up swap chain and the back buffer.
 
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

	void getGpuInfo(std::string& description, int& memory);
	
	ID3D11DepthStencilView* getDepthStencilView();

	void turnZBufferOn();
	void turnZBufferOff();

	void setBackBufferRenderTarget();
	void resetViewport(float width, float height);

private:
	bool m_vsyncEnabled;
	int m_gpuMemory;
	std::string m_gpuDescription;
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterizerState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
	ID3D11DepthStencilState* m_disableDepthStencilState;	
};

