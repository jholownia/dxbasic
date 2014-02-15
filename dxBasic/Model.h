/*
================================
 Model.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include "mesh.h"
#include "texturearray.h"
#include "Moveable.h"


#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
#include <vector>

/*
================================
 
 Model

 A generic 3D object which can be loaded from .OBJ files.
 
================================
*/
class Model : public Moveable
{
public:
	Model(void);	
	virtual ~Model(void);

	bool init(ID3D11Device* device, const std::string& meshFilename, const std::string& textureFilename, const std::string& bumpMapFilename, const std::string& specularMapFilename);
	void shutdown();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount() const;

	ID3D11ShaderResourceView** getTextureArray();  //change to vector

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	TextureArray* m_textureArray;
	std::vector<Mesh> m_mesh;

	bool initBuffers(ID3D11Device* device);
	void renderBuffers(ID3D11DeviceContext* deviceContext);
	void shutdownBuffers();	
	bool loadTextures(ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3);
	void releaseTextures();
	bool loadMesh(const std::string& filename);
	bool loadMeshFromObj(const std::string& filename);
			
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 bitangent;
	};

	struct TempVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	
	void CalculateTBNVectors();	
};

