/*
================================
 object3d.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "Model.h"
#include "objloader.h"

#include <iostream>
#include <string>


/*
================
 Object3D::Object3D
================
*/
Object3D::Object3D(void) :
	vertexBuffer_ (NULL),
	indexBuffer_  (NULL),
	textureArray_ (NULL),
	mesh_         (NULL)
{

}

/*
================
 Object3D::~Object3D
================
*/
Object3D::~Object3D(void)
{

}

/*
================
 Object3D::init

 Creates a 3d object given direct 3d device, .obj mesh filename and texture filename.
================
*/
bool Object3D::init( ID3D11Device* device, char* meshFilename, WCHAR* textureFilename, WCHAR* bumpMapFilename, WCHAR* specularMapFilename )
{
	bool result;

	// Load mesh
	{
		std::string s(meshFilename);

		if (s.find(".obj") == std::string::npos)
		{
			result = loadMesh(meshFilename);
		}
		else
		{
			result = loadMeshFromObj(meshFilename);
		}
	}	
	if (!result)
	{
		return false;
	}
	
	result = initBuffers(device);
	if (!result)
	{
		return false;
	}

	result = loadTextures(device, textureFilename, bumpMapFilename, specularMapFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 Object3D::shutdown
================
*/
void Object3D::shutdown()
{
	releaseTextures();
	shutdownBuffers();
	releaseMesh();
}

/*
================
 Object3D::render
================
*/
void Object3D::render( ID3D11DeviceContext* deviceContext )
{
	renderBuffers(deviceContext);
}

/*
================
 Object3D::getIndexCount
================
*/
int Object3D::getIndexCount() const
{
	return indexCount_;
}

/*
================
 Object3D::getTexture
================
*/
ID3D11ShaderResourceView** Object3D::getTexturesArray()
{
	return textureArray_->getTextureArray();
}

/*
================
 Object3D::initBuffers

 Creates vertex and index buffers.
================
*/
bool Object3D::initBuffers( ID3D11Device* device )
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	vertices = new Vertex[vertexCount_];
	indices = new unsigned long[indexCount_];

	for (int i = 0; i < vertexCount_; i++)
	{
		vertices[i].position = D3DXVECTOR3(mesh_[i].x, mesh_[i].y, mesh_[i].z);
		vertices[i].texture = D3DXVECTOR2(mesh_[i].tu, mesh_[i].tv);
		vertices[i].normal = D3DXVECTOR3(mesh_[i].nx, mesh_[i].ny, mesh_[i].nz);
		vertices[i].tangent = D3DXVECTOR3(mesh_[i].tx, mesh_[i].ty, mesh_[i].tz);
		vertices[i].binormal = D3DXVECTOR3(mesh_[i].bx, mesh_[i].by, mesh_[i].bz);

		indices[i] = i;
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount_;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer_);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount_;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer_);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = NULL;

	delete [] indices;
	indices = NULL;

	return true;
}

/*
================
 Object3D::renderBuffers
================
*/
void Object3D::renderBuffers( ID3D11DeviceContext* deviceContext )
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*
================
 Object3D::shutdownBuffers
================
*/
void Object3D::shutdownBuffers()
{
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = NULL;
	}

	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = NULL;
	}
}

/*
================
 Object3D::loadTexture
================
*/
bool Object3D::loadTextures( ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3 )
{
	textureArray_ = new TextureArray;

	return textureArray_->init(device, filename1, filename2, filename3);
}

/*
================
 Object3D::releaseTexture
================
*/
void Object3D::releaseTextures()
{
	if (textureArray_)
	{
		textureArray_->shutdown();
		delete textureArray_;
		textureArray_ = NULL;
	}
}

void Object3D::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertex vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	faceCount = vertexCount_ / 3;

	index = 0;

	for ( i = 0; i < faceCount; ++i)
	{
		vertex1.x = mesh_[index].x;
		vertex1.y = mesh_[index].y;
		vertex1.z = mesh_[index].z;
		vertex1.tu = mesh_[index].tu;
		vertex1.tv = mesh_[index].tv;
		vertex1.nx = mesh_[index].nx;
		vertex1.ny = mesh_[index].ny;
		vertex1.nz = mesh_[index].nz;
		++index;

		vertex2.x = mesh_[index].x;
		vertex2.y = mesh_[index].y;
		vertex2.z = mesh_[index].z;
		vertex2.tu = mesh_[index].tu;
		vertex2.tv = mesh_[index].tv;
		vertex2.nx = mesh_[index].nx;
		vertex2.ny = mesh_[index].ny;
		vertex2.nz = mesh_[index].nz;
		++index;

		vertex3.x = mesh_[index].x;
		vertex3.y = mesh_[index].y;
		vertex3.z = mesh_[index].z;
		vertex3.tu = mesh_[index].tu;
		vertex3.tv = mesh_[index].tv;
		vertex3.nx = mesh_[index].nx;
		vertex3.ny = mesh_[index].ny;
		vertex3.nz = mesh_[index].nz;
		++index;

		// Calculate the tangent and binormal of this face
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);
		
		// Calculate new normal using tangent and binormal
		CalculateNormals(tangent, binormal, normal);

		// Store the results back in mesh
		mesh_[index - 1].nx = normal.x;
		mesh_[index - 1].ny = normal.y;
		mesh_[index - 1].nz = normal.z;
		mesh_[index - 1].tx = tangent.x;
		mesh_[index - 1].ty = tangent.y;
		mesh_[index - 1].tz = tangent.z;
		mesh_[index - 1].bx = binormal.x;
		mesh_[index - 1].by = binormal.y;
		mesh_[index - 1].bz = binormal.z;

		mesh_[index - 2].nx = normal.x;
		mesh_[index - 2].ny = normal.y;
		mesh_[index - 2].nz = normal.z;
		mesh_[index - 2].tx = tangent.x;
		mesh_[index - 2].ty = tangent.y;
		mesh_[index - 2].tz = tangent.z;
		mesh_[index - 2].bx = binormal.x;
		mesh_[index - 2].by = binormal.y;
		mesh_[index - 2].bz = binormal.z;

		mesh_[index - 3].nx = normal.x;
		mesh_[index - 3].ny = normal.y;
		mesh_[index - 3].nz = normal.z;
		mesh_[index - 3].tx = tangent.x;
		mesh_[index - 3].ty = tangent.y;
		mesh_[index - 3].tz = tangent.z;
		mesh_[index - 3].bx = binormal.x;
		mesh_[index - 3].by = binormal.y;
		mesh_[index - 3].bz = binormal.z;
	}
}

void Object3D::CalculateTangentBinormal(TempVertex vertex1, TempVertex vertex2, TempVertex vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;
	
	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate denominator for the tangent/binormal equation
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross product and multiply by the coefficient to get tangent and binormal
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;	
}

void Object3D::CalculateNormals(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;	
}


/*
================
 Object3D::loadMesh

 Loads mesh data from a simple text file with
 the following format:
  
 Vertex Count: nnn

 Data:
 x, y, z, tu, tv, nx, ny, nz
 x, y, z, tu, tv, nx, ny, nz
 ...
================
*/
bool Object3D::loadMesh( char* filename )
{
	std::ifstream ifs;
	
	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	char input;
	
	// Get to vertex count
	ifs.get(input);
	while(input != ':')
	{
		ifs.get(input);
	}

	// read vertex count
	ifs >> vertexCount_;
	indexCount_ = vertexCount_;

	// Create mesh
	mesh_ = new Mesh[vertexCount_];

	// Read to the beginning of the data
	ifs.get(input);
	while(input != ':')
	{
		ifs.get(input);
	}
	ifs.get(input);
	ifs.get(input);

	// Read the data
	for (int i = 0; i < vertexCount_; ++i)
	{
		ifs >> mesh_[i].x >> mesh_[i].y >> mesh_[i].z;
		ifs >> mesh_[i].tu >> mesh_[i].tv;
		ifs >> mesh_[i].nx >> mesh_[i].ny >> mesh_[i].nz;		
	}

	ifs.close();

	return true;
}

/*
================
 Object3D::loadMeshFromObj
================
*/
bool Object3D::loadMeshFromObj( char* filename )
{
	bool result;
	
	ObjLoader loader;
	result = loader.init(filename);
	mesh_ = loader.createMesh(vertexCount_);
	loader.shutdown();

	indexCount_ = vertexCount_;

	return result;
}

/*
================
 Object3D::releaseMesh
================
*/
void Object3D::releaseMesh()
{	
	delete mesh_;
	mesh_ = NULL;	
}


