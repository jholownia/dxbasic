/*
================================
 scenemanager.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "scenemanager.h"

/*
================
 SceneManager::SceneManager
================
*/
SceneManager::SceneManager(void) :
	d3d_	(NULL),
	camera_	(NULL),
	model_	(NULL),
	shader_	(NULL),
	light_	(NULL)
{

}

/*
================
 SceneManager::~SceneManager
================
*/
SceneManager::~SceneManager(void)
{

}

/*
================
 SceneManager::init

 All the objects are initialized here.
================
*/
bool SceneManager::init( int screenWidth, int screenHeight, HWND hwnd)
{	
	bool result;
	
	d3d_ = new D3DManager;

	if (!d3d_)
	{
		return false;
	}

	result = d3d_->init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Error initializing Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create camera
	camera_ = new Camera;
	camera_->setPosition(0.0f, 2.0f, -5.0f);
	camera_->setRotation(20.0f, 0.0f, 0.0f);
	camera_->render();
	D3DXMATRIX baseViewMatrix;
	camera_->getViewMatrix(baseViewMatrix);

	// Create model
	model_ = new Object3D;
	result = model_->init(d3d_->getDevice(), "data/ball2.obj", L"data/earth_4k.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Model.", L"Error", MB_OK);
		return false;
	}

	model_->setRotation(0.0f, 45.0f, 0.0f);

	// Create shader
	shader_ = new Shader;
	result = shader_->init(d3d_->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Shader.", L"Error", MB_OK);
		return false;
	}

	// Light
	light_ = new Light;
	light_->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light_->setDiffuseColor(1.0f, 1.0f, 0.7f, 1.0f);	
	// light_->setDirection(-1.0f, 0.7f, 1.5f);
	
	light_->setDirection(-1.0f, -1.0f, 1.0f);
	light_->setLookAt(0.0f, 0.0f, 0.0f);	
	light_->generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	
	light_->setSpecularColor(1.0f, 1.0f, 0.8f, 1.0f);
	light_->setSpecularPower(44.0f);
	light_->setSpecularIntensity(0.7f);
	
	return true;
}

/*
================
 SceneManager::shutdown
================
*/
void SceneManager::shutdown()
{
	delete light_;
	light_ = NULL;

	if (model_)
	{
		model_->shutdown();
		delete model_;
		model_ = NULL;
	}

	delete camera_;
	camera_ = NULL;

	if (d3d_)
	{
		d3d_->shutdown();
		delete d3d_;
		d3d_ = NULL;
	}
}

/*
================
 SceneManager::frame

 This method is called every frame from App object
 and sets up all the variables before rendering.
================
*/
bool SceneManager::frame()
{			
	// camera_->setPosition();
	// camera_->setRotation();

	// model_->setPosition();
	// model_->setRotation();
	
	// WTF?
	// Light position

	/*static float lightPositionX = -5.0f;
	lightPositionX += 0.05f;

	if (lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	light_->setPosition(lightPositionX, 8.0f, -5.0f);*/
	
	return render();
}

/*
================
 SceneManager::render

 All rendering takes place here
================
*/
bool SceneManager::render()
{	
	renderScene();	
	
	return true;
}


/*
================
 SceneManager::renderScene

 Renders the scene to current render target.
================
*/
void SceneManager::renderScene()
{
	// Begin scene
	d3d_->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render the camera
	camera_->render();

	// Get matrices
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;

	camera_->getViewMatrix(viewMatrix);
	d3d_->getProjectionMatrix(projectionMatrix);
	d3d_->getOrthoMatrix(orthoMatrix);

	// Move the model?
	static float rotation = 0;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, -rotation);
	rotation += 0.01f;
	if (rotation == 360) rotation = 0;

	//D3DXMatrixRotationY(&rotationMatrix, model_->getRotation().y);	
	//D3DXMATRIX translationMatrix;
	//D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);
	
	// Render the model

	D3DXMATRIX worldMatrix;
	d3d_->getWorldMatrix(worldMatrix);

	// Scale?
	/*D3DXMATRIX scaleMatrix;	
	D3DXMatrixScaling(&scaleMatrix, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);*/

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);

	model_->render(d3d_->getDeviceContext());
	shader_->render(d3d_->getDeviceContext(), model_->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model_->getTexture(), light_->getDirection(), light_->getAmbientColor(), light_->getDiffuseColor(), camera_->getPosition(), light_->getSpecularColor(), light_->getSpecularPower(), light_->getSpecularIntensity());

	// End scene
	d3d_->endScene();
}
