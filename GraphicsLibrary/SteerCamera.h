#pragma once

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include <math\Matrix.h>
#include <math\Vec3.h>

class SteerCamera
{
private:
	sm::Vec3 position;

	float roll;
	float yaw;
	float pitch;

	POINT *lastMousePos;
	POINT screenCentre;
	POINT lastPos;

public:
	SteerCamera();
	~SteerCamera();

	void Process(float ms);
	void SetViewMatrix();
	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();
};
