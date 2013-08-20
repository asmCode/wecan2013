#pragma once

#include "../Math/Vec3.h"
#include "../Math/Matrix.h"

class BoundingBox;
class Vertex;

class BoundingSphere
{
public:
	float radius;
	sm::Vec3 center;

	static BoundingSphere FromBBox(const BoundingBox &bbox);
	static bool IsCollision(const BoundingSphere &bs1, const BoundingSphere &bs2);
	BoundingSphere Transform(const sm::Matrix &transform);
};
