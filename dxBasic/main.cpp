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
 WinMain
================
*/
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	std::unique_ptr<App> app(new App);		
	
	if (app->init())
	{
		app->run();
	}	

	return 0;
}