#include "Utils.h"

Utils::Utils(void)
{
}

Utils::~Utils(void)
{
}

sm::Vec3 Utils::CalcTangent(Vertex *v0, Vertex *v1, Vertex *v2)
{
	float s1 = v1 ->texCoord.x - v0 ->texCoord.x;
	float t1 = v1 ->texCoord.y - v0 ->texCoord.y;
	float s2 = v2 ->texCoord.x - v0 ->texCoord.x;
	float t2 = v2 ->texCoord.y - v0 ->texCoord.y;

	sm::Vec3 Q1 = v1 ->position - v0 ->position;
	sm::Vec3 Q2 = v2 ->position - v0 ->position;

	float detInv = 1.0f / (s1 * t2 - s2 * t1);

	sm::Vec3 T(
		detInv * (t2 * Q1.x - t1 * Q2.x),
		detInv * (t2 * Q1.y - t1 * Q2.y),
		detInv * (t2 * Q1.z - t1 * Q2.z));

	T.Normalize();

	return T;
}

void Utils::CalcTangents(Vertex *vertices, int triangleCount, sm::Vec3 *tangents)
{
	for (int i = 0; i < triangleCount; i++)
	{
		int baseIndex = i * 3;

		for (int j = 0; j < 3; j++)
		{
			int p0Index = baseIndex + ((0 + j) % 3);
			int p1Index = baseIndex + ((1 + j) % 3);
			int p2Index = baseIndex + ((2 + j) % 3);

			sm::Vec3 T = CalcTangent(
				&vertices[p0Index],
				&vertices[p1Index],
				&vertices[p2Index]);

			tangents[baseIndex + j] = T;

			//sm::Vec3 B = (vertices[p0Index].normal * T).GetNormalized();

			/*sm::Vec3 B(
			detInv * (-s2 * Q1.x + s1 * Q2.x),
			detInv * (-s2 * Q1.y + s1 * Q2.y),
			detInv * (-s2 * Q1.z + s1 * Q2.z));*/
		}
	}
}