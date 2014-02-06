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
	input_			(NULL),
	scene_			(NULL),
	cameraPosition_	(NULL),
	modelPosition_	(NULL)
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
	input_ = new InputManager;
	if (!input_)
	{
		return false;
	}
	
	result = input_->init(hinstance_, hwnd_, screenWidth, screenHeight);
	if (!result)
	{
		MessageBoxW(hwnd_, L"Could not initialize input manager", L"Error", MB_OK);
		return false;
	}

	// Create scene
	scene_ = new SceneManager;
	if(!scene_)
	{
		return false;
	}

	result = scene_->init(screenWidth, screenHeight, hwnd_);
	if(!result)
	{
		return false;
	}

	return true;

	// Camera position
	cameraPosition_ = new Position;
	cameraPosition_->setPosition(0.0f, 5.0f, 0.0f);
	cameraPosition_->setRotation(0.0f, 45.0f, 0.0f);

	// Model position
	modelPosition_ = new Position;
	modelPosition_->setPosition(10.0f, 1.0f, 10.0f);
	modelPosition_->setRotation(0.0f, 45.0f, 0.0f);

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
	delete modelPosition_;
	modelPosition_ = NULL;

	delete cameraPosition_;
	cameraPosition_ = NULL;	
	
	if(scene_)
	{
		scene_->shutdown();
		delete scene_;
		scene_ = NULL;
	}

	if(input_)
	{
		input_->shutdown();
		delete input_;
		input_ = NULL;
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

		if (input_->isEscPressed() == true)
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

	result = input_->frame();
	if (!result)
	{
		return false;
	}

	// Pass parameters to scene manager
	result = scene_->frame();
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
		
	appHandle = this;
	hinstance_ = GetModuleHandle(NULL);
		
	appName_ = L"Tiger";

	// Setup windows
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance_;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = appName_;
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
	hwnd_ = CreateWindowExW(WS_EX_APPWINDOW, appName_, appName_, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance_, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd_, SW_SHOW);
	SetForegroundWindow(hwnd_);
	SetFocus(hwnd_);

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
	DestroyWindow(hwnd_);
	hwnd_ = NULL;

	// Remove the application instance.
	UnregisterClass(appName_, hinstance_);
	hinstance_ = NULL;

	// Release the pointer to this class.
	appHandle = NULL;	
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
			return appHandle->messageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
