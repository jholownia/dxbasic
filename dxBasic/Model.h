/*
================================
 object3d.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>

#include "mesh.h"
#include "texturearray.h"
#include "Moveable.h"


/*
================================
 
 Object3D

 A generic 3D object which can be loaded from .OBJ files.
 
================================
*/
class Object3D : public Moveable
{
public:
	Object3D(void);	
	virtual ~Object3D(void);

	bool init(ID3D11Device* device, char*, WCHAR* textureFilename, WCHAR* bumpMapFilename, WCHAR* specularMapFilename);
	void shutdown();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount() const;

	ID3D11ShaderResourceView** getTexturesArray();

private:
	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;
	int vertexCount_;
	int indexCount_;
	TextureArray* textureArray_;
	Mesh* mesh_;

	bool initBuffers(ID3D11Device* device);
	void renderBuffers(ID3D11DeviceContext* deviceContext);
	void shutdownBuffers();	
	bool loadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3);
	void releaseTextures();
	bool loadMesh(char* filename);
	bool loadMeshFromObj(char* filename);
	void releaseMesh();
		
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct TempVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertex, TempVertex, TempVertex, VectorType&, VectorType& );
	void CalculateNormals(VectorType, VectorType, VectorType& );

};

