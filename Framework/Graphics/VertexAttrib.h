#pragma once

#include <stdint.h>

class VertexAttrib
{
public:
	static const uint8_t Position = 0;
	static const uint8_t Coords1 = 1;
	static const uint8_t Coords2 = 2;
	static const uint8_t Coords3 = 3;
	static const uint8_t Normal = 4;
	static const uint8_t Tangent = 5;

	static const uint8_t VertexAttribsAmount = 6;
};

