#pragma once

#include <math\Vec3.h>
#include <math\Matrix.h>

class BoundingBox;
class Vertex;

class BoundingSphere
{
public:
	float radius;
	sm::Vec3 center;

	static BoundingSphere FromVertices(const Vertex *vertices, unsigned count);
	static BoundingSphere FromBBox(const BoundingBox &bbox);
	static bool IsCollision(const BoundingSphere &bs1, const BoundingSphere &bs2);
	BoundingSphere Transform(const sm::Matrix &transform);
};
