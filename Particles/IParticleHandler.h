#pragma once

#include <Math/Matrix.h>
#include <stdint.h>

class Particle;

class IParticleHandler
{
public:
	virtual ~IParticleHandler() {};
	
	virtual Particle* CreateParticle() = 0;

	// call before drawing any particle
	virtual void Setup() {};

	virtual void Draw(Particle *particles) = 0;

	// call after drawing all particles
	virtual void Clean() {};

	virtual void SetMetrices(const sm::Matrix &modelViewMatrix, const sm::Matrix &projMatrix) = 0;
};

