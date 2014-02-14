/*
================================
 fpscounter.h
 
 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

/*
================================
 
 FpsCounter

 Counts the number of frames rendered each second.
 
================================
*/
class FpsCounter
{
public:
	FpsCounter(void);
	~FpsCounter(void);

	void init();
	void update();
	int getFps();

private:
	int m_fps;
	int m_config;
	unsigned long m_startTime;
};