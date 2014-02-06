/*
================================
 main.cpp

 WinMain

 Created on: 18 Jan 2014
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2014.
================================
*/

#include "app.h"

/*
================

================
*/
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	App* app;
	bool result;

	// Create application object
	app = new App;

	if (!app)
	{
		return 0;	
	}

	// Run the application
	result = app->init();
	
	if (result)
	{
		app->run();
	}

	// Shutdown and clean up
	app->shutdown();	
	delete app;
	app = NULL;

	return 0;
}