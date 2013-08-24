#pragma once

#include "../Math/Vec3.h"
#include "../Math/Vec2.h"
#include <stdint.h>

class VertexPC2NT
{
public:
	static const uint8_t Channels;

	sm::Vec3 position;
	sm::Vec2 coords1;
	sm::Vec2 coords2;
	sm::Vec3 normal;
	sm::Vec3 tangent;
};

