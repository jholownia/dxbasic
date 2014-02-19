/*
================================
 app.h

 Created on: 18 Jan 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

#pragma once

#pragma warning( disable : 4005 )

// This should speed up build process
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>

class SceneManager;
class InputManager;
class Position;


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
	void run();

	LRESULT CALLBACK messageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	LPCWSTR m_appName;    
	HINSTANCE m_hinstance;
	HWND m_hwnd;          
	std::unique_ptr<InputManager> m_input;
	std::unique_ptr<SceneManager> m_scene;
	std::unique_ptr<Position> m_cameraPosition;
	std::unique_ptr<Position> m_modelPosition;
	
	bool update();
	void initializeWindows(int& width, int& height);
	void shutdownWindows();
};

static App* g_appHandle = NULL;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);