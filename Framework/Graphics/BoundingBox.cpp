#include "BoundingBox.h"
#include "Vertex.h"
#include "VertexInformation.h"
#include <Math/Vec3.h>
#include <assert.h>

BoundingBox BoundingBox::FromVertices(const void *vertices, unsigned count, uint8_t vertexType, float scale)
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

	sm::Vec3 v0Pos = VertexInformation::GetPosition(vertices, vertexType);

	bbox.maxX = v0Pos.x;
	bbox.minX = v0Pos.x;
	bbox.maxY = v0Pos.y;
	bbox.minY = v0Pos.y;
	bbox.maxZ = v0Pos.z;
	bbox.minZ = v0Pos.z;

	for (unsigned i = 0; i < count; i++)
	{
		sm::Vec3 viPos = VertexInformation::GetPosition(vertices, vertexType);

		if (bbox.minX > viPos.x) bbox.minX = viPos.x;
		if (bbox.maxX < viPos.x) bbox.maxX = viPos.x;
		if (bbox.minY > viPos.y) bbox.minY = viPos.y;
		if (bbox.maxY < viPos.y) bbox.maxY = viPos.y;
		if (bbox.minZ > viPos.z) bbox.minZ = viPos.z;
		if (bbox.maxZ < viPos.z) bbox.maxZ = viPos.z;

		vertices = reinterpret_cast<const uint8_t*>(vertices) + VertexInformation::GetStride(vertexType);
	}

	bbox.maxX *= scale;
	bbox.minX *= scale;
	bbox.maxY *= scale;
	bbox.minY *= scale;
	bbox.maxZ *= scale;
	bbox.minZ *= scale;

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
