#pragma once

#include "../Math/Matrix.h"
#include "../Math/Vec3.h"
#include "../Math/Vec2.h"
#include <Windows.h>
#include <gl/GL.h>
#include <glext.h>

class SteerCamera
{
private:
	sm::Vec3 position;

	float roll;
	float yaw;
	float pitch;

	sm::Vec2 *lastMousePos;
	sm::Vec2 screenCentre;
	sm::Vec2 lastPos;

public:
	SteerCamera();
	~SteerCamera();

	void Process(float ms);
	void SetViewMatrix();
	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();
};
