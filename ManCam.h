#pragma once

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

#include <math\Matrix.h>
#include <math\Vec3.h>
#include "GraphicsLibrary\ICamera.h"

class ManCam : public ICamera
{
private:
	float roll;
	float yaw;
	float pitch;

	POINT *lastMousePos;
	POINT screenCentre;
	POINT lastPos;

public:
	ManCam();
	~ManCam();

	sm::Vec3 position;

	void Process(float ms);

	void SetViewMatrix();
	sm::Matrix GetViewMatrix();

	sm::Vec3 GetPosition(float time);

	float GetFov(float time);
	float GetFocal(float time);
};
