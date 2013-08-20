#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include <assert.h>

BoundingSphere BoundingSphere::FromBBox(const BoundingBox &bbox)
{
	BoundingSphere bs;

	bs.center = bbox.center;
	bs.radius = (bbox.center - bbox.GetBBoxVerticle(0)).GetLength();

	return bs;
}

bool BoundingSphere::IsCollision(const BoundingSphere &bs1, const BoundingSphere &bs2)
{
	return (bs1.center - bs2.center).GetLength() < (bs1.radius + bs2.radius);
}

BoundingSphere BoundingSphere::Transform(const sm::Matrix &transform)
{
	assert(false && "not implemented");
	//TODO: tu pewnie sa zle obliczenia. Do radiusa trzeba dodac center
	BoundingSphere bs;

	/*sm::Vec3 radiusVec(radius, 0, 0);
	radiusVec = transform * radiusVec;
	bs.center = transform * center;
	bs.radius = (bs.center - radius).GetLength();*/

	return bs;
}