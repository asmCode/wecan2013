#pragma once

#include <stdint.h>

class VertexAttribBit
{
public:
	static const uint8_t Position =	0x1;
	static const uint8_t Coords1 =	0x2;
	static const uint8_t Coords2 =	0x4;
	static const uint8_t Coords3 =	0x8;
	static const uint8_t Normal =	0x10;
	static const uint8_t Tangent =	0x20;
};

