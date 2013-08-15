#pragma once

#include "../Math/Vec3.h"

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

	static BoundingBox FromVertices(const Vertex *vertices, unsigned count);

	sm::Vec3 GetBBoxVerticle(unsigned index) const;
	void SetBBoxVerticle(unsigned index, const sm::Vec3 &v);
};
