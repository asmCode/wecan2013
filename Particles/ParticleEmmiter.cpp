#include "ParticleEmmiter.h"
#include "IParticleHandler.h"
#include "Particle.h"
#include <Graphics/Interpolators/InterpolatorFactory.h>
#include <Math/Vec4.h>
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
	m_initialMaxSpeed(DefaultInitialMaxSpeed),
	m_sourceType(SourceType_Cone)
{
	assert(m_particleHandler != NULL);

	m_speedOverLifeTime = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_Linear);
	m_sizeOverLifeTime = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_Linear);
	m_colorOverLifeTime = InterpolatorFactory::CreateInterpolator<sm::Vec4>(InterpolatorFactory::InterpolationMethod_Linear);

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

	uint32_t particlesProcessed = 0;

	for (uint32_t i = 0; i < m_maxParticesCount; i++)
	{
		if (particlesProcessed >= m_activeParticles && m_sparksToStart < 1.0)
			break;

		// spark is living
		if (m_particles[i]->m_isUsed && m_particles[i]->m_liteTime < m_particles[i]->m_liteTimeLimit)
		{
			UpdateParticle(m_particles[i], seconds);

			particlesProcessed++;
		}
		// spark just died
		else if (m_particles[i]->m_isUsed && m_particles[i]->m_liteTime >= m_particles[i]->m_liteTimeLimit)
		{
			m_activeParticles--;
			m_particles[i]->m_isUsed = false;
		}

		if (m_sparksSourceEnabled && !m_particles[i]->m_isUsed && m_sparksToStart >= 1.0f)
		{
			m_activeParticles++;
			StartParticle(m_particles[i]);
			UpdateParticle(m_particles[i], seconds);
			m_sparksToStart -= 1.0f;
		}
	}

	m_particleHandler->SetMetrices(m_viewMatrix, m_projMatrix);
}

void ParticleEmmiter::Draw(float seconds)
{
	if (m_activeParticles == 0)
		return;

	m_particleHandler->Setup();

	uint32_t particlesProcessed = 0;

	for (uint32_t i = 0; i < m_maxParticesCount; i++)
	{
		if (particlesProcessed >= m_activeParticles)
			break;

		if (m_particles[i]->m_isUsed)
		{
			m_particleHandler->Draw(m_particles[i]);

			particlesProcessed++;
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

void ParticleEmmiter::SetLifeTime(float minLifetime, float maxLifetime)
{
	m_minLifetime = minLifetime;
	m_maxLifetime = maxLifetime;
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

	sm::Vec3 position;
	sm::Vec3 direction;

	if (m_sourceType == SourceType_Plane)
	{
		float base1Scale = random.GetFloat();
		float base2Scale = random.GetFloat();

		position = m_sourcePosition + (m_planeSourceBase1 * base1Scale + m_planeSourceBase2 * base2Scale);
		direction = m_direction;
	}
	else if (m_sourceType == SourceType_Cone)
	{
		sm::Vec3 spreadVector = sm::Vec3(
			random.GetFloat(-m_spreadAngle, m_spreadAngle),
			random.GetFloat(-m_spreadAngle, m_spreadAngle),
			random.GetFloat(-m_spreadAngle, m_spreadAngle));

		position = m_sourcePosition;
		direction = (m_direction + spreadVector).GetNormalized();
	}

	float speed = random.GetFloat(m_initialMinSpeed, m_initialMaxSpeed);

	particle->m_position = position;
	particle->m_moveDirection = direction;
	particle->m_size = 4.0f;
	particle->m_color = sm::Vec4(1, 1, 1, 1);
	particle->m_liteTime = 0.0f;
	particle->m_liteTimeLimit = random.GetFloat(m_minLifetime, m_maxLifetime);
	particle->m_isUsed = true;
}

void ParticleEmmiter::SetViewMatrix(const sm::Matrix &viewMatrix)
{
	m_viewMatrix = viewMatrix;
}

void ParticleEmmiter::SetProjMatrix(const sm::Matrix &projMatrix)
{
	m_projMatrix = projMatrix;
}

void ParticleEmmiter::SetGravityVelocity(const sm::Vec3 &gravityVelocity)
{
	m_gravityVelocity = gravityVelocity;
}

void ParticleEmmiter::SetSpeedOverLifetime(float begin, float end)
{
	m_speedOverLifeTime->AddKeyframe(0.0f, begin, false);
	m_speedOverLifeTime->AddKeyframe(1.0f, end, false);
}

void ParticleEmmiter::SetSizeOverLifetime(float begin, float end)
{
	m_sizeOverLifeTime->AddKeyframe(0.0f, begin, false);
	m_sizeOverLifeTime->AddKeyframe(1.0f, end, false);
}

void ParticleEmmiter::SetColorOverLifetime(const sm::Vec4 &begin, const sm::Vec4 &end)
{
	m_colorOverLifeTime->AddKeyframe(0.0f, begin, false);
	m_colorOverLifeTime->AddKeyframe(1.0f, end, false);
}

void ParticleEmmiter::UpdateParticle(Particle *particle, float seconds)
{
	if (particle->m_liteTime < 0.0f)
		particle->m_liteTime = 0.0f;

	if (particle->m_liteTime > particle->m_liteTimeLimit)
		particle->m_liteTime = particle->m_liteTimeLimit;

	float timeNormalized = particle->m_liteTime / particle->m_liteTimeLimit;

	float speed;
	m_speedOverLifeTime->GetValue(timeNormalized, speed);
	
	particle->m_position += ((particle->m_moveDirection * speed) + m_gravityVelocity) * seconds;
	m_colorOverLifeTime->GetValue(timeNormalized, particle->m_color);
	m_sizeOverLifeTime->GetValue(timeNormalized, particle->m_size);

	particle->m_liteTime += seconds;
}

void ParticleEmmiter::SetSourceAsCone(const sm::Vec3 &position, const sm::Vec3 &direction, float spreadAngle)
{
	m_sourceType = SourceType_Cone;

	m_sourcePosition = position;
	m_direction = direction;
	m_spreadAngle = spreadAngle;
}

void ParticleEmmiter::SetSourceAsPlane(const sm::Vec3 &cornerPosition, const sm::Vec3 &base1, const sm::Vec3 &base2)
{
	m_sourceType = SourceType_Plane;

	m_sourcePosition = cornerPosition;
	m_planeSourceBase1 = base1;
	m_planeSourceBase2 = base2;
}

