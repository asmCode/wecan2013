#pragma once

#include "../Math/Vec3.h"
#include <stdint.h>

class VertexType
{
public:
	static const uint8_t P = 0;
	static const uint8_t PN = 1;
	static const uint8_t PCN = 2;
	static const uint8_t PC2N = 3;
	static const uint8_t PCNT = 4;
	static const uint8_t PC2NT = 5;

	static const uint8_t VertexTypesAmount = 6;
};

