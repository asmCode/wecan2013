#pragma once

#include <Math/Matrix.h>

class IDrawable
{
public:
	virtual void Draw(float time, float seconds) = 0;
};

