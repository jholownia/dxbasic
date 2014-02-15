/*
================================
 scenemanager.h
 
 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

#include <Windows.h>

// Use forward declarations and move all this to the cpp file

#include "Config.h"
#include "D3DManager.h"
#include "camera.h"
#include "model.h"
#include "shader.h"
#include "light.h"


/*
================================
 
 SceneManager
 
 Main class for rendering the scene.
 Contains pointers to all the objects in the scene.

================================
*/
class SceneManager
{
public:
	SceneManager(void);
	SceneManager(SceneManager const&);
	~SceneManager(void);

	bool init(int, int, HWND);
	void shutdown();
	bool frame();
	bool render();

private:	
	D3DManager* d3d_;
	Camera* camera_;
	Model* model_;
	Shader* shader_;
	Light* light_;

	void renderScene();
};

