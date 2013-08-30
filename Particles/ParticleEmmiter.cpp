#include "ParticleEmmiter.h"
#include "IParticleHandler.h"
#include "Particle.h"
#include <Utils/Randomizer.h>
#include <memory>
#include <assert.h>

const uint32_t ParticleEmmiter::DefaultSparksPerSecond = 10;
const float ParticleEmmiter::DefaultInitialMinSpeed = 5.0f;
const float ParticleEmmiter::DefaultInitialMaxSpeed = 10.0f;
const float ParticleEmmiter::DefaultSpreadAngle = 1.0f;

ParticleEmmiter::ParticleEmmiter(uint32_t maxSpritesCount,
								 IParticleHandler *particleHandler) :
	m_maxParticesCount(maxSpritesCount),
	m_particleHandler(particleHandler),
	m_activeParticles(0),
	m_sparksSourceEnabled(true),
	m_liveSparksCount(0),
	m_sparksToStart(0.0f),
	m_sourcePosition(sm::Vec3(0, 0, 0)),
	m_spreadAngle(DefaultSpreadAngle),
	m_sparksPerSeconds(DefaultSparksPerSecond),
	m_initialMinSpeed(DefaultInitialMinSpeed),
	m_initialMaxSpeed(DefaultInitialMaxSpeed)
{
	assert(m_particleHandler != NULL);

	m_particles= new Particle*[m_maxParticesCount];
	
	for (uint32_t i = 0; i < m_maxParticesCount; i++)
		m_particles[i] = m_particleHandler->CreateParticle();
}

ParticleEmmiter::~ParticleEmmiter(void)
{
	for (uint32_t i = 0; i < m_maxParticesCount; i++)
		delete m_particles[i];

	delete [] m_particles;
}

void ParticleEmmiter::Update(float seconds)
{
	if (m_sparksSourceEnabled)
		m_sparksToStart += static_cast<float>(m_sparksPerSeconds) * seconds;

	if (m_activeParticles == 0 && m_sparksToStart < 1.0f)
		return;

	for (uint32_t i = 0; i < m_maxParticesCount; i++)
	{
		// spark is living
		if (m_particles[i]->m_isUsed && m_particles[i]->m_liteTime >= 0.0f)
		{
			m_particles[i]->m_liteTime -= seconds;
			//m_particles[i]->m_throw.Update(seconds);
		}
		// spark just died
		else if (m_particles[i]->m_isUsed && m_particles[i]->m_liteTime < 0.0f)
		{
			m_activeParticles--;
			m_particles[i]->m_isUsed = false;
		}

		if (m_sparksSourceEnabled && !m_particles[i]->m_isUsed && m_sparksToStart >= 1.0f)
		{
			m_activeParticles++;
			StartParticle(m_particles[i]);
			m_sparksToStart -= 1.0f;
		}
	}
}

void ParticleEmmiter::Draw(float seconds)
{
	if (m_activeParticles == 0)
		return;

	m_particleHandler->Setup();

	for (uint32_t i = 0; i < m_maxParticesCount; i++)
	{
		if (m_particles[i]->m_isUsed)
		{
			float power = 1.0f;
			if (m_particles[i]->m_liteTime < 0.5f && m_particles[i]->m_liteTime >= 0.0f)
			{
				power = m_particles[i]->m_liteTime / 0.5f;
				assert(power >= 0.0f && power <= 1.0f);
			}
			if (m_particles[i]->m_liteTime <= 0.0f)
				power = 0.0f;

			m_particleHandler->Draw(m_particles[i]);
		}
	}

	m_particleHandler->Clean();
}

void ParticleEmmiter::SetSourcePosition(const sm::Vec3 &position)
{
	m_sourcePosition = position;
}

void ParticleEmmiter::SetSourceDirection(const sm::Vec3 &direction, float spreadAngle)
{
	m_direction = direction;
	m_spreadAngle = spreadAngle;
}

void ParticleEmmiter::SetInitialSpeed(float minSpeed, float maxSpeed)
{
	m_initialMinSpeed = minSpeed;
	m_initialMaxSpeed = maxSpeed;
}

void ParticleEmmiter::SetSparksPerSecond(float sparksPerSecond)
{
	m_sparksPerSeconds = sparksPerSecond;
}

void ParticleEmmiter::EnableSparksSource()
{
	m_sparksSourceEnabled = true;
}

void ParticleEmmiter::DisableSparksSource()
{
	m_sparksSourceEnabled = false;
	m_sparksToStart = 0.0f;
}

void ParticleEmmiter::StartParticle(Particle *particle)
{
	static Randomizer random;

	sm::Vec3 spreadVector = sm::Vec3(
		random.GetFloat(-m_spreadAngle, m_spreadAngle),
		random.GetFloat(-m_spreadAngle, m_spreadAngle),
		random.GetFloat(-m_spreadAngle, m_spreadAngle));

	float speed = random.GetFloat(m_initialMinSpeed, m_initialMaxSpeed);

	//particle->m_throw.Throw(m_sourcePosition, m_direction + spreadVector, speed, 30.0f);
	particle->m_liteTime = random.GetFloat(0.4f, 1.2f);
	particle->m_isUsed = true;
}

