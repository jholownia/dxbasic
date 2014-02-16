/*
================================
 app.cpp

 Main application object.

 Created on: 18 Jan	2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

#include "app.h"

/*
================
 App::App
================
*/
App::App(void) :
	m_input			(NULL),
	m_scene			(NULL),
	m_cameraPosition	(NULL),
	m_modelPosition	(NULL)
{

}

/*
================
 App::~App
================
*/
App::~App(void)
{
	
}

/*
================
 App::init

 Initializes Windows, DirectX, input and sound.
================
*/
bool App::init()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	// Initialize Windows API
	initializeWindows(screenWidth, screenHeight);

	// Create input object
	m_input = new InputManager;
	if (!m_input)
	{
		return false;
	}
	
	result = m_input->init(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(m_hwnd, L"Could not initialize input manager", L"Error", MB_OK);
		return false;
	}

	// Create scene
	m_scene = new SceneManager;
	if(!m_scene)
	{
		return false;
	}

	result = m_scene->init(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}

	return true;

	// Camera position
	m_cameraPosition = new Position;
	m_cameraPosition->setPosition(0.0f, 5.0f, 0.0f);
	m_cameraPosition->setRotation(0.0f, 45.0f, 0.0f);

	// Model position
	m_modelPosition = new Position;
	m_modelPosition->setPosition(10.0f, 1.0f, 10.0f);
	m_modelPosition->setRotation(0.0f, 45.0f, 0.0f);

	return true;
}

/*
================
 App::shutdown

 Cleans up.
================
*/
void App::shutdown()
{	
	delete m_modelPosition;
	m_modelPosition = NULL;

	delete m_cameraPosition;
	m_cameraPosition = NULL;	
	
	if(m_scene)
	{
		m_scene->shutdown();
		delete m_scene;
		m_scene = NULL;
	}

	if(m_input)
	{
		m_input->shutdown();
		delete m_input;
		m_input = NULL;
	}

	shutdownWindows();
}

/*
================
 App::run

 Runs the application message loop.
================
*/
void App::run()
{
	MSG msg;	
	bool result;

	// Initialize message structure
	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;

	while(!done)
	{
		// Handle windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Check for quit message
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Process the frame
			result = frame();
			
			if (!result)
			{
				done = true;
			}
		}

		if (m_input->isEscPressed() == true)
		{
			done = true;
		}
	}

}

/*
================
 App::frame

 Does all per frame processing.
================
*/
bool App::frame()
{
	bool result;

	result = m_input->update();
	if (!result)
	{
		return false;
	}

	// Pass parameters to scene manager
	result = m_scene->frame();
	if (!result)
	{
		return false;
	}

	return true;
}

/*
================
 App::messageHandler

 Sends all messages which are not key presses to default handler.
================
*/
LRESULT CALLBACK App::messageHandler( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

/*
================
 App::initializeWindows 
================
*/
void App::initializeWindows( int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;
		
	g_appHandle = this;
	m_hinstance = GetModuleHandle(NULL);
		
	m_appName = L"Tiger";

	// Setup windows
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(false)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowExW(WS_EX_APPWINDOW, m_appName, m_appName, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

/*
================
 App::shutdownWindows
================
*/
void App::shutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(false)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_appName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	g_appHandle = NULL;	
}

/*
================
 WndProc

 Windows procedure.
================
*/
LRESULT CALLBACK WndProc( HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return g_appHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
