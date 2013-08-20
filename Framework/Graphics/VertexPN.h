#pragma once

#include "../Math/Vec3.h"
#include "../Math/Vec2.h"
#include <stdint.h>

class VertexPN
{
public:
	static const uint8_t Channels;

	sm::Vec3 position;
	sm::Vec3 normal;
};

