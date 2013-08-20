#include "VertexInformation.h"

#include "Vertex.h"
#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <assert.h>

int VertexInformation::m_vertexSizes[] =
{
	sizeof(sm::Vec3),
	sizeof(sm::Vec3),
};

//int VertexInformation::m_offsets[][];

int VertexInformation::GetStride(uint8_t vertexChannels)
{
	if (vertexChannels == VertexP::Channels)
		return sizeof(VertexP);
	else if (vertexChannels == VertexPN::Channels)
		return sizeof(VertexPN);
	else if (vertexChannels == VertexPCN::Channels)
		return sizeof(VertexPCN);
	else if (vertexChannels == VertexPC2N::Channels)
		return sizeof(VertexPC2N);
	else if (vertexChannels == VertexPCNT::Channels)
		return sizeof(VertexPCNT);

	assert(false);
	return 0;
}

int VertexInformation::GetOffset(uint8_t vertexChannels, VertexChannel channel)
{
	assert(false);
	return 0;
}

sm::Vec3 VertexInformation::GetPosition(const void *vertex, uint8_t channels)
{
	if (channels == VertexP::Channels)
		return reinterpret_cast<const VertexP*>(vertex)->position;
	else if (channels == VertexPN::Channels)
		return reinterpret_cast<const VertexPN*>(vertex)->position;
	else if (channels == VertexPCN::Channels)
		return reinterpret_cast<const VertexPCN*>(vertex)->position;
	else if (channels == VertexPC2N::Channels)
		return reinterpret_cast<const VertexPC2N*>(vertex)->position;
	else if (channels == VertexPCNT::Channels)
		return reinterpret_cast<const VertexPCNT*>(vertex)->position;

	assert(false);
	return sm::Vec3();
}

