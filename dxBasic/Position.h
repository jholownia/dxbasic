/*
================================
 position.cpp

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include <math.h>


/*
================================
 
 Position

 Class for framerate independent position calculations.
 
================================
*/
class Position
{
public:
	Position(void);
	~Position(void);

	void setPosition(float, float, float);
	void setRotation(float, float, float);

	void getPosition(float&, float&, float&);
	void getRotation(float&, float&, float&);
	void getYRotation(float&);
		
	void setFrameTime(float); 
	
	void turnLeft(bool);
	void turnRight(bool);

	void moveForward(bool);
	void moveBack(bool);

	void strafeLeft(bool);
	void strafeRight(bool);	

private:
	float m_posX, m_posY, m_posZ;
	float m_rotationX, m_rotationY, m_rotationZ;	
	float m_frameTime;	
	float m_leftTurnSpeed;
	float m_rightTurnSpeed;
	float m_forwardSpeed, m_backwardSpeed;
	float m_strafeRightSpeed, m_strafeLeftSpeed;
};

