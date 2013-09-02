#include "ParticlesManager.h"
#include "DemoController.h"
#include "DistortParticleHandler.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Utils/StringUtils.h>
#include <assert.h>

ParticlesManager::ParticlesManager()
{
}

ParticlesManager::~ParticlesManager()
{
}

void ParticlesManager::Initialize(Model *modelWithParticles)
{
	m_demo = DemoController::GetInstance();
	Content *content = m_demo->m_content;

	Texture *smokeTexture = content->Get<Texture>("smoke");
	assert(smokeTexture != NULL);

	Shader *distortionEffect = content->Get<Shader>("DistortParticle");
	assert(distortionEffect != NULL);

	m_distortParticleHandler = new DistortParticleHandler(distortionEffect, smokeTexture, NULL);

	std::vector<Mesh*> &m_meshes = modelWithParticles->GetMeshes();
	for (uint32_t i = 0; i < m_meshes.size(); i++)
	{
		std::string id;
		std::string type;
		if (ParseName(m_meshes[i]->name, id, type))
		{
			assert(id.size() > 0);
			assert(type.size() > 0);

			std::map<std::string, ParticlesSource*>::iterator it = m_particlesSources.find(id);
			if (it == m_particlesSources.end())
				m_particlesSources[id] = new ParticlesSource();

			ParticlesSource *particleSource = m_particlesSources[id];

			if (type == "src")
				particleSource->m_position = m_meshes[i]->m_worldMatrix * sm::Vec3(0, 0, 0);
			else if (type == "dir")
				particleSource->m_direction = m_meshes[i]->m_worldMatrix * sm::Vec3(0, 0, 0);
		}
	}

	std::map<std::string, ParticlesSource*>::iterator it = m_particlesSources.begin();
	for (; it != m_particlesSources.end(); it++)
	{
		sm::Vec3 direction = it->second->m_direction - it->second->m_position;

		ParticleEmmiter *emiter = new ParticleEmmiter(1000, m_distortParticleHandler);
		emiter = new ParticleEmmiter(1000, m_distortParticleHandler);
		emiter->SetSourcePosition(it->second->m_position);
		emiter->SetSparksPerSecond(10);
		emiter->SetLifeTime(3.0f, 8.0f);
		emiter->SetSizeOverLifetime(1.0f, 10.0f);
		emiter->SetGravityVelocity(sm::Vec3(0, 3, 0));
		emiter->SetColorOverLifetime(sm::Vec4(1, 1, 1, 0.5), sm::Vec4(1, 1, 1, 0));
		emiter->SetSpeedOverLifetime(direction.GetLength(), 0.0f);
		emiter->SetSourceDirection(direction.GetNormalized(), 0.1f);

		m_emmiters.push_back(emiter);
	}
}

void ParticlesManager::Update(float seconds)
{
	for (uint32_t i = 0; i < m_emmiters.size(); i++)
	{
		m_emmiters[i]->SetViewMatrix(m_viewMatrix);
		m_emmiters[i]->SetProjMatrix(m_projMatrix);
		m_emmiters[i]->Update(seconds);
	}
}

void ParticlesManager::Draw()
{
	for (uint32_t i = 0; i < m_emmiters.size(); i++)
		m_emmiters[i]->Draw(0.0f);
}

bool ParticlesManager::ParseName(const std::string &meshName, std::string &id, std::string &type)
{
	std::string particleToken = "particles";

	if (meshName.size() > particleToken.size() &&
		meshName.substr(0, particleToken.size()) == particleToken)
	{
		std::vector<std::string> values;
		StringUtils::Split(meshName, ".", values);
		if (values.size() == 3)
		{
			id = values[1];
			type = values[2];

			return true;
		}
	}

	return false;
}

void ParticlesManager::SetViewMatrix(const sm::Matrix &viewMatrix)
{
	m_viewMatrix = viewMatrix;
}

void ParticlesManager::SetProjMatrix(const sm::Matrix &projMatrix)
{
	m_projMatrix = projMatrix;
}

