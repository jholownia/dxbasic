/*
================================
 app.h

 Created on: 18 Jan 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

#pragma once

// This should speed up build process
#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN

#include "scenemanager.h"
#include "inputmanager.h"
#include "position.h"

#include <Windows.h>


/*
================================
 
 App

 Main application class. Create and destroys all the other objects,
 handles input and controls graphics and sound.
 
================================
*/
class App
{
public:
	App(void);	
	~App(void);

	bool init();
	void shutdown();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR appName_;    
	HINSTANCE hinstance_;
	HWND hwnd_;          
	InputManager* input_;
	SceneManager* scene_;
	Position* cameraPosition_;
	Position* modelPosition_;
	
	bool frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

};

static App* appHandle = NULL;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);