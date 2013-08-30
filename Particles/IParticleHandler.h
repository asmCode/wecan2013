#pragma once

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
};

