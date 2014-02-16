/*
================================
 shader.cpp
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "shader.h"
#include <D3Dcompiler.h>

/*
================
 Shader::Shader
================
*/
Shader::Shader(void) :
	m_vertexShader        (NULL),
	m_pixelShader         (NULL),
	m_layout              (NULL),
	m_matrixBuffer        (NULL),
	m_samplerState        (NULL),
	m_cameraBuffer        (NULL),
	m_lightBuffer         (NULL)
{

}

/*
================
 Shader::~Shader
================
*/
Shader::~Shader(void)
{
}

/*
================
 Shader::init
================
*/
bool Shader::init( ID3D11Device* device, HWND hwnd )
{
	return initShader(device, hwnd, L"phong.vs", L"phong.ps" );
}

/*
================
 Shader::shutdown
================
*/
void Shader::shutdown()
{
	shutdownShader();
}

/*
================
 Shader::render
================
*/
void Shader::render( ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<ID3D11ShaderResourceView*> textureArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, float specularIntensity)
{
	setShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor, specularPower, specularIntensity);
	renderShader(deviceContext, indexCount);
}

/*
================
 Shader::initShader
================
*/
bool Shader::initShader( ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename )
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;	


	// Initialize the error message.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Load the shaders in from the files.
	
	// Compile vertex shader
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, D3DCOMPILE_DEBUG, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	
	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile pixel shader
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, D3DCOMPILE_DEBUG, NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage)
		{
			outputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create pixel shader from the buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}
		
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType structure in the Model class and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;


	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
		
	// Create the input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	// Setup a description for the dynamic matrix constant buffer in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	// Camera buffer
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBuffer);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Light Buffer
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false; /// !!!
	}

	
	//// Get pointers to the three matrices inside the shader so we can update them from this class.
	//worldMatrixPtr_ = effect_->GetVariableByName("worldMatrix")->AsMatrix();
	//viewMatrixPtr_ = effect_->GetVariableByName("viewMatrix")->AsMatrix();
	//projectionMatrixPtr_ = effect_->GetVariableByName("projectionMatrix")->AsMatrix();

	//// Pointer to the texture resource inside the shader
	//texturePrt_ = effect_->GetVariableByName("shaderTexture")->AsShaderResource();

	//// Light direction, ambient and diffuse color
	//lightDirectionPtr_ = effect_->GetVariableByName("lightDirection")->AsVector();
	//ambientColorPtr_ = effect_->GetVariableByName("ambientColor")->AsVector();
	//diffuseColorPtr_ = effect_->GetVariableByName("diffuseColor")->AsVector();

	//// Specular components
	//cameraPositionPtr_ = effect_->GetVariableByName("cameraPosition")->AsVector();
	//specularColorPtr_ = effect_->GetVariableByName("specularColor")->AsVector();
	//specularPowerPtr_ = effect_->GetVariableByName("specularPower")->AsScalar();
	//specularIntensityPtr_ = effect_->GetVariableByName("specularIntensity")->AsScalar();

	return true;
}

/*
================
 Shader::shutdownShader
================
*/
void Shader::shutdownShader()
{
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL;
	}

	if (m_cameraBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = NULL;
	}		
	
	if (m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = NULL;
	}
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = NULL;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = NULL;
	}	
	
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = NULL;
	}

	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}	
	
}

/*
================
 Shader::outputShaderErrorMessage
================
*/
void Shader::outputShaderErrorMessage( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename )
{
	char* compileErrors;
	unsigned long bufferSize;
	std::ofstream ofs;

	compileErrors = (char *)(errorMessage->GetBufferPointer());
	bufferSize = errorMessage->GetBufferSize();

	ofs.open("shader_errors.txt");

	for (unsigned int i = 0; i < bufferSize; ++i)
	{
		ofs << compileErrors[i];
	}

	ofs.close();

	errorMessage->Release();
	errorMessage = NULL;

	MessageBox(hwnd, L"Error compiling shader. Check shader_errors.txt for details.", filename, MB_OK);
}

/*
================
 Shader::setShaderParameters
================
*/
bool Shader::setShaderParameters( ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<ID3D11ShaderResourceView*> texturesArray, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower, float specularIntensity )
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBuffer* dataPtr;
	CameraBuffer* dataPtr2;
	LightBuffer* dataPtr3;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the buffer
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock constant buffer so it can be written to
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBuffer*)mappedResource.pData;

	// Copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 3, &texturesArray[0]);

	// Camera Buffer
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (CameraBuffer*) mappedResource.pData;
	dataPtr2->cameraPostion = cameraPosition;
	dataPtr2->padding = 0.0f;

	deviceContext->Unmap(m_cameraBuffer, 0);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// Set shader texture resource in the pixel shader
	// deviceContext->PSSetShaderResources(0, 1, &texturesArray);

	// Light buffer
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (LightBuffer*)mappedResource.pData;
	dataPtr3->ambientColor = ambientColor;
	dataPtr3->diffuseColor = diffuseColor;
	dataPtr3->lightDirection = lightDirection;
	dataPtr3->specularColor = specularColor;
	dataPtr3->specularPower = specularPower;
	dataPtr3->specularIntensity = specularIntensity;
	/*dataPtr3->padding1 = 0.0f;
	dataPtr3->padding2 = 0.0f;
	dataPtr3->padding3 = 0.0f;*/

	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	
	return true;
	

	//// Set the world matrix variable inside the shader.
	//worldMatrixPtr_->SetMatrix((float*)&worldMatrix);

	//// Set the view matrix variable inside the shader.
	//viewMatrixPtr_->SetMatrix((float*)&viewMatrix);

	//// Set the projection matrix variable inside the shader.
	//projectionMatrixPtr_->SetMatrix((float*)&projectionMatrix);

	//// Bind the texture
	//texturePrt_->SetResource(texture);

	//// Light
	//lightDirectionPtr_->SetFloatVector((float *) &lightDirection);
	//ambientColorPtr_->SetFloatVector((float *) &ambientColor);
	//diffuseColorPtr_->SetFloatVector((float *) &diffuseColor);

	//// Camera position
	//cameraPositionPtr_->SetFloatVector((float *) &cameraPosition);

	//// Specular color
	//specularColorPtr_->SetFloatVector((float*) &specularColor);

	//// Specular power
	//specularPowerPtr_->SetFloat(specularPower);

	//// Specular intensity
	//specularIntensityPtr_->SetFloat(specularIntensity);
}

/*
================
 Shader::renderShader
================
*/
void Shader::renderShader( ID3D11DeviceContext* deviceContext, int indexCount )
{	
	// Set the input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set vertex and pixel shaders which will be used for rendering this triangle
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	// Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
