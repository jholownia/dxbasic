/*
================================
 scenemanager.cpp

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "scenemanager.h"
#include "D3DManager.h"
#include "camera.h"
#include "model.h"
#include "shader.h"
#include "light.h"
#include "Config.h"

/*
================
 SceneManager::SceneManager
================
*/
SceneManager::SceneManager(void) :
	m_d3d	 (nullptr),
	m_camera (nullptr),
	m_model	 (nullptr),
	m_shader (nullptr),
	m_light	 (nullptr)
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
	
	m_d3d.reset(new D3DManager);

	result = m_d3d->init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Error initializing Direct3D", L"Error", MB_OK);
		return false;
	}

	m_d3d->resetViewport(400, 800);
	m_d3d->resetViewport(400, 600);
	m_d3d->resetViewport(800, 300);
	
	// Create camera
	m_camera.reset(new Camera);

	// camera_->setPosition(0.0f, 2.0f, -5.0f);
	// camera_->setRotation(20.0f, 0.0f, 0.0f);
	m_camera->setPosition(0.0f, 0.0f, -5.0f);
	m_camera->render();
	D3DXMATRIX baseViewMatrix;
	m_camera->getViewMatrix(baseViewMatrix);

	// Create model
	m_model.reset(new Model);
	result = m_model->init(m_d3d->getDevice(), "data/ball.obj", "data/earth_4k.jpg", "data/earth_normal.png", "data/earth_specular.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Model.", L"Error", MB_OK);
		return false;
	}

	m_model->setRotation(0.0f, 45.0f, 0.0f);

	// Create shader
	m_shader.reset(new Shader);
	result = m_shader->init(m_d3d->getDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Shader.", L"Error", MB_OK);
		return false;
	}

	// Light
	m_light.reset(new Light);
	m_light->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_light->setDiffuseColor(1.0f, 1.0f, 0.8f, 1.0f);	
	// light_->setDirection(-1.0f, 0.7f, 1.5f);
	
	m_light->setDirection(0.0f, 0.0f, 1.0f);
	// light_->setDirection(-0.8f, -0.5f, 1.0f);
	m_light->setLookAt(0.0f, 0.0f, 0.0f);	
	m_light->generateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	
	m_light->setSpecularColor(1.0f, 1.0f, 0.7f, 1.0f);
	m_light->setSpecularPower(26.0f);
	m_light->setSpecularIntensity(0.6f);
	
	return true;
}

/*
================
 SceneManager::frame

 This method is called every frame from App object
 and sets up all the variables before rendering.
================
*/
bool SceneManager::update()
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
	m_d3d->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render the camera
	m_camera->render();

	// Get matrices
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix;

	m_camera->getViewMatrix(viewMatrix);
	m_d3d->getProjectionMatrix(projectionMatrix);
	m_d3d->getOrthoMatrix(orthoMatrix);

	// Move the model?
	static float rotation = 0;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationY(&rotationMatrix, -rotation);
	rotation += 0.01f;
	if (rotation >= 360.0f) rotation -= 360.0f;

	//D3DXMatrixRotationY(&rotationMatrix, model_->getRotation().y);	
	//D3DXMATRIX translationMatrix;
	//D3DXMatrixTranslation(&translationMatrix, tiger_->getPosition().x, tiger_->getPosition().y, tiger_->getPosition().z);
	
	// Render the model

	D3DXMATRIX worldMatrix;
	m_d3d->getWorldMatrix(worldMatrix);

	// Scale?
	/*D3DXMATRIX scaleMatrix;	
	D3DXMatrixScaling(&scaleMatrix, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);*/

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotationMatrix);

	m_model->render(m_d3d->getDeviceContext());
	m_shader->render(m_d3d->getDeviceContext(), m_model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_model->getTextureArray(), m_light->getDirection(), m_light->getAmbientColor(), m_light->getDiffuseColor(), m_camera->getPosition(), m_light->getSpecularColor(), m_light->getSpecularPower(), m_light->getSpecularIntensity());

	// End scene
	m_d3d->endScene();
}
