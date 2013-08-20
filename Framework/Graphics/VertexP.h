#pragma once

#include "../Math/Vec3.h"
#include <stdint.h>

class VertexP
{
public:
	static const uint8_t Channels;

	sm::Vec3 position;
};

