#include "BoundingBox.h"
#include <assert.h>

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
