#include "BoundingBox.h"
#include "Vertex.h"
#include <assert.h>

BoundingBox BoundingBox::FromVertices(const Vertex *vertices, unsigned count)
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

	bbox.maxX = vertices[0].position.x;
	bbox.minX = vertices[0].position.x;
	bbox.maxY = vertices[0].position.y;
	bbox.minY = vertices[0].position.y;
	bbox.maxZ = vertices[0].position.z;
	bbox.minZ = vertices[0].position.z;

	for (unsigned i = 0; i < count; i++)
	{
		if (bbox.minX > vertices[i].position.x) bbox.minX = vertices[i].position.x;
		if (bbox.maxX < vertices[i].position.x) bbox.maxX = vertices[i].position.x;
		if (bbox.minY > vertices[i].position.y) bbox.minY = vertices[i].position.y;
		if (bbox.maxY < vertices[i].position.y) bbox.maxY = vertices[i].position.y;
		if (bbox.minZ > vertices[i].position.z) bbox.minZ = vertices[i].position.z;
		if (bbox.maxZ < vertices[i].position.z) bbox.maxZ = vertices[i].position.z;
	}

	bbox.center.Set(
		(bbox.maxX + bbox.minX) / 2.0f,
		(bbox.maxY + bbox.minY) / 2.0f,
		(bbox.maxZ + bbox.minZ) / 2.0f);

	return bbox;
}

sm::Vec3 BoundingBox::GetBBoxVerticle(unsigned index) const
{
	switch (index)
	{
	case 0: return sm::Vec3(minX, maxY, minZ);
	case 1: return sm::Vec3(maxX, maxY, minZ);
	case 2: return sm::Vec3(maxX, maxY, maxZ);
	case 3: return sm::Vec3(minX, maxY, maxZ);
	case 4: return sm::Vec3(minX, minY, minZ);
	case 5: return sm::Vec3(maxX, minY, minZ);
	case 6: return sm::Vec3(maxX, minY, maxZ);
	case 7: return sm::Vec3(minX, minY, maxZ);
	}

	assert(false && "box has only 8 vertices, stupid...");
	return sm::Vec3();
}

void BoundingBox::SetBBoxVerticle(unsigned index, const sm::Vec3 &v)
{
	switch (index)
	{
	case 0: minX = v.x; maxY = v.y; minZ = v.z; break;
	case 1: maxX = v.x; maxY = v.y; minZ = v.z; break;
	case 2: maxX = v.x; maxY = v.y; maxZ = v.z; break;
	case 3: minX = v.x; maxY = v.y; maxZ = v.z; break;
	case 4: minX = v.x; minY = v.y; minZ = v.z; break;
	case 5: maxX = v.x; minY = v.y; minZ = v.z; break;
	case 6: maxX = v.x; minY = v.y; maxZ = v.z; break;
	case 7: minX = v.x; minY = v.y; maxZ = v.z; break;
	}
}
