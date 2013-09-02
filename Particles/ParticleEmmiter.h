#pragma once

#include "Particle.h"
#include <Graphics/Interpolators/IInterpolator.h>
#include <Math\Vec3.h>
#include <Math\Matrix.h>
#include <stdint.h>

class IParticleHandler;
class Particle;

class ParticleEmmiter
{
public:
	enum SourceType
	{
		SourceType_Cone,
		SourceType_Plane
	};

	ParticleEmmiter(
		uint32_t maxParticlesCount,
		IParticleHandler *particleHandler);

	~ParticleEmmiter(void);

	void Update(float seconds);
	void Draw(float seconds);

	void SetSourceAsCone(const sm::Vec3 &position, const sm::Vec3 &direction, float spreadAngle);
	void SetSourceAsPlane(const sm::Vec3 &cornerPosition, const sm::Vec3 &base1, const sm::Vec3 &base2);
	void SetSourcePosition(const sm::Vec3 &position);
	void SetSourceDirection(const sm::Vec3 &direction, float spreadAngle); // spreadAngle is a fake, should be cone angle
	void SetInitialSpeed(float minSpeed, float maxSpeed);
	void SetSparksPerSecond(float sparksPerSecond);
	void SetLifeTime(float maxLifetime, float minLifetime);
	void SetViewMatrix(const sm::Matrix &viewMatrix);
	void SetProjMatrix(const sm::Matrix &projMatrix);

	void SetGravityVelocity(const sm::Vec3 &gravityVelocity);

	void SetSpeedOverLifetime(float begin, float end);
	void SetSizeOverLifetime(float begin, float end);
	void SetColorOverLifetime(const sm::Vec4 &begin, const sm::Vec4 &end);

	void EnableSparksSource();
	void DisableSparksSource();

private:
	static const uint32_t DefaultSparksPerSecond;
	static const float DefaultInitialMinSpeed;
	static const float DefaultInitialMaxSpeed;
	static const float DefaultSpreadAngle;

	SourceType m_sourceType;

	sm::Matrix	m_viewMatrix;
	sm::Matrix	m_projMatrix;

	uint32_t m_maxParticesCount;
	uint32_t m_activeParticles; // performance purpose

	IParticleHandler *m_particleHandler;

	bool m_sparksSourceEnabled;

	sm::Vec3 m_sourcePosition;
	sm::Vec3 m_direction;
	float m_spreadAngle;
	uint32_t m_sparksPerSeconds;
	float m_initialMinSpeed;
	float m_initialMaxSpeed;

	sm::Vec3 m_planeSourceBase1;
	sm::Vec3 m_planeSourceBase2;

	float m_minLifetime;
	float m_maxLifetime;

	IInterpolator<float> *m_speedOverLifeTime;
	IInterpolator<sm::Vec4> *m_colorOverLifeTime;
	IInterpolator<float> *m_sizeOverLifeTime;

	uint32_t m_liveSparksCount;
	float m_sparksToStart;

	sm::Vec3 m_gravityVelocity;

	Particle **m_particles;

	void StartParticle(Particle *particle);
	void UpdateParticle(Particle *particle, float seconds);
};

