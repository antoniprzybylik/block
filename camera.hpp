#pragma once

#include <windows.h>

int cameraX;
int cameraY;
	
void SetCameraX(int CameraX)
{
	
	cameraX=CameraX;
	
}

void SetCameraY(int CameraY)
{
			
	cameraY=CameraY;
	
}
		
int GetCameraX()
{
		
	return cameraX;
	
}

int GetCameraY()
{
	
	return cameraY;
	
}

POINT GetCameraPosition()
{
		
	POINT position;
	position.x=cameraX;
	position.y=cameraY;
			
	return position;
		
}

void SetCameraPosition(POINT position)
{
	
	cameraX=position.x;
	cameraY=position.y;
	
}

void SetCameraPosition(int 	CameraX, int CameraY)
{
	
	cameraX=CameraX;
	cameraY=CameraY;
	
}
