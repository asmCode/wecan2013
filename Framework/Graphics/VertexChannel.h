#pragma once

enum VertexChannel : int
{
	VertexChannel_Position = 0x1,
	VertexChannel_Coords1 = 0x2,
	VertexChannel_Coords2 = 0x4,
	VertexChannel_Coords3 = 0x8,
	VertexChannel_Normal = 0x10,
	VertexChannel_Tangent = 0x20
};

