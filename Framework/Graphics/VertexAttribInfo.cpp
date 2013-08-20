#pragma once

#include "VertexChannel.h"
#include "../Math/Vec3.h"
#include <stdint.h>

class VertexInformation
{
public:
	// in bytes
	static int GetStride(uint8_t vertexChannels);
	static int GetOffset(uint8_t vertexChannels, VertexChannel channel);
	static sm::Vec3 GetPosition(const void *vertex, uint8_t channels);

private:
	static int m_vertexSizes[];
//	static int m_offsets[][];


};

