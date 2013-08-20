#pragma once

#include <windows.h>
#include <stdint.h>
#include <math\Vec3.h>
#include <Graphics/VertexInformation.h>

class Vertex;

class BoundingBox
{
public:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	sm::Vec3 center;

	static BoundingBox FromVertices(const void *vertices, unsigned count, uint8_t vertexChannels)
	{
		BoundingBox bbox;

		//assert(count > 0);
		if (count == 0)
		{
			bbox.maxX = 0;
			bbox.minX = 0;
			bbox.maxY = 0;
			bbox.minY = 0;
			bbox.maxZ = 0;
			bbox.minZ = 0;
			bbox.center.Set(0, 0, 0);

			return bbox;
		}

		sm::Vec3 v0Pos = VertexInformation::GetPosition(vertices, vertexChannels);

		bbox.maxX = v0Pos.x;
		bbox.minX = v0Pos.x;
		bbox.maxY = v0Pos.y;
		bbox.minY = v0Pos.y;
		bbox.maxZ = v0Pos.z;
		bbox.minZ = v0Pos.z;

		for (unsigned i = 0; i < count; i++)
		{
			vertices = reinterpret_cast<const uint8_t*>(vertices) + VertexInformation::GetStride(vertexChannels);

			sm::Vec3 viPos = VertexInformation::GetPosition(vertices, vertexChannels);
			if (bbox.minX > viPos.x) bbox.minX = viPos.x;
			if (bbox.maxX < viPos.x) bbox.maxX = viPos.x;
			if (bbox.minY > viPos.y) bbox.minY = viPos.y;
			if (bbox.maxY < viPos.y) bbox.maxY = viPos.y;
			if (bbox.minZ > viPos.z) bbox.minZ = viPos.z;
			if (bbox.maxZ < viPos.z) bbox.maxZ = viPos.z;
		}

		bbox.center.Set(
			(bbox.maxX + bbox.minX) / 2.0f,
			(bbox.maxY + bbox.minY) / 2.0f,
			(bbox.maxZ + bbox.minZ) / 2.0f);

		return bbox;
	}

	sm::Vec3 GetBBoxVerticle(unsigned index) const;
	void SetBBoxVerticle(unsigned index, const sm::Vec3 &v);
};
