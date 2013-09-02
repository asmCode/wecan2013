#pragma once

#include "VertexType.h"
#include "VertexAttrib.h"
#include "../Math/Vec3.h"
#include <stdint.h>

class VertexInformation
{
public:
	// in bytes
	static int GetStride(uint8_t vertexType);

	// in bytes
	static int GetOffset(uint8_t vertexType, uint8_t vertexAttrib);

	static sm::Vec3 GetPosition(const void *vertex, uint8_t vertexType);
	static sm::Vec3 GetPosition(const void *vertices, uint32_t index, uint8_t vertexType);

	static bool HasAttrib(uint8_t vertexType, uint8_t vertexAttrib);

private:
	static int m_vertexStrides[VertexType::VertexTypesAmount];
	static int m_vertexOffsets[VertexType::VertexTypesAmount][VertexAttrib::VertexAttribsAmount];
};

