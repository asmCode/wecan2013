#pragma once

#include "../Math/Vec3.h"
#include "../Math/Vec2.h"
#include <stdint.h>

class VertexPCN
{
public:
	static const uint8_t Channels;

	sm::Vec3 position;
	sm::Vec2 coords;
	sm::Vec3 normal;
};

