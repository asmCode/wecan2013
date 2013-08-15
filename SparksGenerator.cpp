#include "SparksGenerator.h"
#include "DrawingRoutines.h"
#include <assert.h>

SparksGenerator::SparksGenerator(void) :
m_sparksSourceEnabled(true),
	m_liveSparksCount(0),
	m_sparksToStart(0.0f),
	m_sourcePosition(sm::Vec3(0, 0, 0))
{
}


SparksGenerator::~SparksGenerator(void)
{
}

void SparksGenerator::Update(float seconds)
{
	if (m_sparksSourceEnabled)
		m_sparksToStart += static_cast<float>(SparksPerSecond) * seconds;

	for (uint32_t i = 0; i < MaxSparks; i++)
	{
		// spark is living
		if (m_sparks[i].m_isUsed && m_sparks[i].m_liteTime >= 0.0f)
		{
			m_sparks[i].m_liteTime -= seconds;
			m_sparks[i].m_position += m_sparks[i].m_moveTarget * seconds * 40.0f;
			m_sparks[i].m_moveTarget = (m_sparks[i].m_moveTarget + (sm::Vec3(0, -9.8f, 0.0f) * seconds * 2.0f));
		}
		// spark just died
		else if (m_sparks[i].m_isUsed && m_sparks[i].m_liteTime < 0.0f)
				 m_sparks[i].m_isUsed = false;

		if (m_sparksSourceEnabled && !m_sparks[i].m_isUsed && m_sparksToStart >= 1.0f)
		{
			m_sparks[i].Start(m_sourcePosition);
			m_sparksToStart -= 1.0f;
		}
	}
}

void SparksGenerator::Draw(float seconds,
						   const sm::Matrix &proj,
						   const sm::Matrix &view)
{
	//return;
	//DrawingRoutines *dr = DrawingRoutines::GetInstance();

	for (uint32_t i = 0; i < MaxSparks; i++)
	{
		if (m_sparks[i].m_isUsed)
		{
			float power = 1.0f;
			if (m_sparks[i].m_liteTime < 0.5f && m_sparks[i].m_liteTime >= 0.0f)
			{
				power = m_sparks[i].m_liteTime / 0.5f;
				assert(power >= 0.0f && power <= 1.0f);
			}
			if (m_sparks[i].m_liteTime <= 0.0f)
				power = 0.0f;

			//dr->DrawSpark(proj, view, m_sparks[i].m_position, m_sparks[i].m_position + m_sparks[i].m_moveTarget * 10.0f, power);
		}
	}
}

void SparksGenerator::SetSourcePosition(const sm::Vec3 &position)
{
	m_sourcePosition = position;
}

void SparksGenerator::EnableSparksSource()
{
	m_sparksSourceEnabled = true;
}

void SparksGenerator::DisableSparksSource()
{
	m_sparksSourceEnabled = false;
	m_sparksToStart = 0.0f;
}

