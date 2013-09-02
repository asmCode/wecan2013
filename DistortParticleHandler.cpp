#include "DistortParticleHandler.h"

#include "DrawingRoutines.h"
#include "Billboard.h"
#include "Particles/Particle.h"
#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <assert.h>

DistortParticleHandler::DistortParticleHandler(Shader *distortParticleShader,
											   Texture *particleTex,
											   Texture *distortTex) :
	m_distortParticleShader(distortParticleShader),
	m_particleTex(particleTex),
	m_distortTex(distortTex)
{
	assert(m_distortParticleShader != NULL);
	assert(m_particleTex != NULL);
	//assert(m_distortTex != NULL);
}


Particle* DistortParticleHandler::CreateParticle()
{
	return new Particle();
}

void DistortParticleHandler::Setup()
{
	m_distortParticleShader->UseProgram();
	m_distortParticleShader->SetTextureParameter("u_particleTex", 0, m_particleTex->GetId());
	m_distortParticleShader->SetMatrixParameter("u_modelViewMatrix", m_modelViewMatrix);
	m_distortParticleShader->SetMatrixParameter("u_projMatrix", m_projMatrix);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

	Billboard::Setup();
}

void DistortParticleHandler::Draw(Particle *particle)
{
	sm::Vec4 position(particle->m_position, particle->m_size);

	m_distortParticleShader->SetParameter("u_particlePosition", position);
	m_distortParticleShader->SetParameter("u_color", particle->m_color);

	Billboard::Draw();
}

void DistortParticleHandler::Clean()
{
	Billboard::Clean();
}


void DistortParticleHandler::SetMetrices(const sm::Matrix &modelViewMatrix, const sm::Matrix &projMatrix)
{
	m_modelViewMatrix = modelViewMatrix;
	m_projMatrix = projMatrix;
}

