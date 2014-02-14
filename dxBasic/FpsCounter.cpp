/*
================================
 fpscounter.h

 Created on: 17 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "fpscounter.h"

/*
================
 FpsCounter::FpsCounter
================
*/
FpsCounter::FpsCounter(void)
{

}

/*
================
 FpsCounter::~FpsCounter
================
*/
FpsCounter::~FpsCounter(void)
{

}

/*
================
 FpsCounter::init
================
*/
void FpsCounter::init()
{
	m_fps = 0;
	m_config = 0;
	m_startTime = timeGetTime();
}

/*
================
 FpsCounter::frame

 Called each frame.
================
*/
void FpsCounter::update()
{
	++m_config;

	// If 1 second has passed count frames since last time
	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_config;
		m_config = 0;

		m_startTime = timeGetTime();
	}
}

/*
================
 FpsCounter::getFps
================
*/
int FpsCounter::getFps()
{
	return m_fps;
}
