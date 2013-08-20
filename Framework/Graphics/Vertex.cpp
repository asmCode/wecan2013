#include "Vertex.h"

const uint8_t VertexP::Channels = VertexChannel_Position;
const uint8_t VertexPN::Channels = VertexChannel_Position | VertexChannel_Normal;
const uint8_t VertexPCN::Channels = VertexChannel_Position | VertexChannel_Coords1 | VertexChannel_Normal;
const uint8_t VertexPC2N::Channels = VertexChannel_Position | VertexChannel_Coords1 | VertexChannel_Coords2 | VertexChannel_Normal;
const uint8_t VertexPCNT::Channels = VertexChannel_Position | VertexChannel_Coords1 | VertexChannel_Normal | VertexChannel_Tangent;
