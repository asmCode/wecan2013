#pragma once

#include <Math/Matrix.h>

class ICamera
{
public:
	virtual sm::Matrix GetViewMatrix() = 0;
	virtual float GetFov(float time) = 0;
};
