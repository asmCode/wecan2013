#pragma once

#include "Particles/IParticleHandler.h"
#include <Math/Matrix.h>

class Texture;
class Shader;
class Particle;

class DistortParticleHandler : public IParticleHandler
{
public:
	DistortParticleHandler(
		Shader *distortParticleShader,
		Texture *particleTex,
		Texture *distortTex);

	Particle* CreateParticle();

	void Setup();
	void Draw(Particle *particles);
	void Clean();

	void SetMetrices(const sm::Matrix &modelViewMatrix, const sm::Matrix &projMatrix);

private:
	Shader *m_distortParticleShader;
	Texture *m_particleTex;
	Texture *m_distortTex;

	sm::Matrix m_modelViewMatrix;
	sm::Matrix m_projMatrix;
};

