#include "Spark.h"

#include <Utils\Randomizer.h>

Spark::Spark() :
	m_isUsed(false)
{
}

void Spark::Start(const sm::Vec3 &position)
{
	static Randomizer random;

	m_position = position;

	m_moveTarget = sm::Vec3(
		random.GetFloat(-0.5f, 0.5f),
		random.GetFloat(0.1f, 1.0f),
		random.GetFloat(-0.5f, 0.5f)).GetNormalized() * random.GetFloat(1.0f, 3.0f);
	m_moveTarget.y *= 2.0f;

	m_liteTime = random.GetFloat(0.4f, 1.2f);

	m_isUsed = true;
}

