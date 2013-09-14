#pragma once

#include <Math/Matrix.h>
#include <Math/Vec3.h>

class ICamera
{
public:
	virtual sm::Matrix GetViewMatrix() = 0;
	virtual float GetFov(float time) = 0;
	virtual sm::Vec3 GetPosition() = 0;

	virtual float GetNearClip() {return 0.1f;}
	virtual float GetFarClip() {return 100.0f;}
};
