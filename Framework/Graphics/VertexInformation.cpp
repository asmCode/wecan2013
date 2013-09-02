#include "VertexInformation.h"

#include "VertexType.h"
#include "VertexAttrib.h"
#include "Vertex.h"
#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <assert.h>

int VertexInformation::m_vertexStrides[] =
{
	sizeof(VertexP),
	sizeof(VertexPN),
	sizeof(VertexPCN),
	sizeof(VertexPC2N),
	sizeof(VertexPCNT),
	sizeof(VertexPC2NT)
};

int VertexInformation::m_vertexOffsets[VertexType::VertexTypesAmount][VertexAttrib::VertexAttribsAmount] =
{
	{ // VertexP
		0,	// position
		-1, // coords1
		-1, // coords2
		-1, // coords3
		-1, // normal
		-1  // tangent
	},

	{ // VertexPN
		0, // position
		-1, // coords1
		-1, // coords2
		-1, // coords3
		sizeof(sm::Vec3), // normal
		-1  // tangent
	},

	{ // VertexPCN
		0, // position
		sizeof(sm::Vec3), // coords1
		-1, // coords2
		-1, // coords3
		sizeof(sm::Vec3) + sizeof(sm::Vec2), // normal
		-1  // tangent
	},

	{ // VertexPC2N
		0, // position
		sizeof(sm::Vec3), // coords1
		sizeof(sm::Vec3) + sizeof(sm::Vec2), // coords2
		-1, // coords3
		sizeof(sm::Vec3) + sizeof(sm::Vec2) + sizeof(sm::Vec2), // normal
		-1  // tangent
	},

	{ // VertexPCNT
		0, // position
		sizeof(sm::Vec3), // coords1
		-1, // coords2
		-1, // coords3
		sizeof(sm::Vec3) + sizeof(sm::Vec2), // normal
		sizeof(sm::Vec3) + sizeof(sm::Vec2)  + sizeof(sm::Vec3)  // tangent
	},

	{ // VertexPC2NT
		0, // position
		sizeof(sm::Vec3), // coords1
		sizeof(sm::Vec3) + sizeof(sm::Vec2), // coords2
		-1, // coords3
		sizeof(sm::Vec3) + sizeof(sm::Vec2) + sizeof(sm::Vec2), // normal
		sizeof(sm::Vec3) + sizeof(sm::Vec2) + sizeof(sm::Vec2)  + sizeof(sm::Vec3)  // tangent
	},
};

int VertexInformation::GetStride(uint8_t vertexType)
{
	return m_vertexStrides[vertexType];
}

int VertexInformation::GetOffset(uint8_t vertexType, uint8_t vertexAttrib)
{
	return m_vertexOffsets[vertexType][vertexAttrib];
}

sm::Vec3 VertexInformation::GetPosition(const void *vertex, uint8_t vertexType)
{
	vertex = reinterpret_cast<const uint8_t*>(vertex) + GetOffset(vertexType, VertexAttrib::Position);

	return *reinterpret_cast<const sm::Vec3*>(vertex);
}

sm::Vec3 VertexInformation::GetPosition(const void *vertices, uint32_t index, uint8_t vertexType)
{
	vertices =
		reinterpret_cast<const uint8_t*>(vertices) +
		(GetStride(vertexType) * index) +
		GetOffset(vertexType, VertexAttrib::Position);

	return *reinterpret_cast<const sm::Vec3*>(vertices);
}

bool VertexInformation::HasAttrib(uint8_t vertexType, uint8_t vertexAttrib)
{
	return GetOffset(vertexType, vertexAttrib) != -1;
}

