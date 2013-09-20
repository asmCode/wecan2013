#include "ParticlesManager.h"
#include "DemoController.h"
#include "DistortParticleHandler.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <Graphics/VertexInformation.h>
#include <Utils/StringUtils.h>
#include <Graphics/Property.h>
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
		///std::string id;
//		std::string type;
		//if (ParseName(m_meshes[i]->name, id, type))
		{
	//		assert(id.size() > 0);
//			assert(type.size() > 0);

			//std::map<std::string, ParticlesSource*>::iterator it = m_particlesSources.find(id);
			//if (it == m_particlesSources.end())
			ParticlesSource *s = new ParticlesSource();
				m_particlesSources.push_back(s);

			ParticlesSource *particleSource = s;

			//if (type == "src")
			{
				assert(m_meshes[i]->meshParts.size() == 1);
				assert(m_meshes[i]->meshParts[0]->verticesCount == 6);

				Property *p_particlesPerSecond = m_meshes[i]->FindProperty("particles_per_second");
				assert(p_particlesPerSecond != NULL);
				Property *p_LifeTimeMin = m_meshes[i]->FindProperty("life_time_min");
				assert(p_LifeTimeMin != NULL);
				Property *p_LifeTimeMax = m_meshes[i]->FindProperty("life_time_max");
				assert(p_LifeTimeMax != NULL);
				Property *p_sizeStart = m_meshes[i]->FindProperty("size_start");
				assert(p_sizeStart != NULL);
				Property *p_sizeEnd = m_meshes[i]->FindProperty("size_end");
				assert(p_sizeEnd != NULL);
				Property *p_opacityStart = m_meshes[i]->FindProperty("opacity_start");
				assert(p_opacityStart != NULL);
				Property *p_opacityEnd = m_meshes[i]->FindProperty("opacity_end");
				assert(p_opacityEnd != NULL);
				Property *p_speedStart = m_meshes[i]->FindProperty("speed_start");
				assert(p_speedStart != NULL);
				Property *p_speedEnd = m_meshes[i]->FindProperty("speed_end");
				assert(p_speedEnd != NULL);

				sm::Vec3 v0 = VertexInformation::GetPosition(m_meshes[i]->meshParts[0]->vertices, 0, m_meshes[i]->meshParts[0]->m_vertexType);
				sm::Vec3 v1 = VertexInformation::GetPosition(m_meshes[i]->meshParts[0]->vertices, 1, m_meshes[i]->meshParts[0]->m_vertexType);
				sm::Vec3 v2 = VertexInformation::GetPosition(m_meshes[i]->meshParts[0]->vertices, 2, m_meshes[i]->meshParts[0]->m_vertexType);

				particleSource->m_position = v0;
				particleSource->m_planeBase1 = v1 - v0;
				particleSource->m_planeBase2 = v2 - v0;
				particleSource->m_direction =
					particleSource->m_planeBase2.GetNormalized() *
					particleSource->m_planeBase1.GetNormalized();
				particleSource->m_speedStart = p_speedStart->GetFloatValue() * 1.0f;
				particleSource->m_speedEnd = p_speedEnd->GetFloatValue();
				particleSource->m_sizeStart = p_sizeStart->GetFloatValue() * 10.0f;
				particleSource->m_sizeEnd = p_sizeEnd->GetFloatValue() * 10;
				particleSource->m_opacityStart = p_opacityStart->GetFloatValue();
				particleSource->m_opacityEnd = p_opacityEnd->GetFloatValue();
				particleSource->m_particlesPersSecond = p_particlesPerSecond->GetIntValue();
				particleSource->m_lifeTimeMin = p_LifeTimeMin->GetFloatValue();
				particleSource->m_lifeTimeMax = p_LifeTimeMax->GetFloatValue();

				//particleSource->m_position = m_meshes[i]->m_worldMatrix * sm::Vec3(0, 0, 0);
				//particleSource->
			}
			//else if (type == "dir")
				//particleSource->m_direction = m_meshes[i]->m_worldMatrix * sm::Vec3(0, 0, 0);
		}
	}

	for (int i = 0; i != m_particlesSources.size(); i++)
	{
		ParticlesSource *ee = m_particlesSources[i];

		//sm::Vec3 direction = it->second->m_direction - it->second->m_position;
		sm::Vec3 direction = ee->m_direction;

		ParticleEmmiter *emiter = new ParticleEmmiter(1000, m_distortParticleHandler);
		emiter = new ParticleEmmiter(1000, m_distortParticleHandler);
		emiter->SetSourcePosition(ee->m_position);
		emiter->SetSparksPerSecond(ee->m_particlesPersSecond);
		emiter->SetLifeTime(ee->m_lifeTimeMin, ee->m_lifeTimeMax);
		emiter->SetSizeOverLifetime(ee->m_sizeStart, ee->m_sizeEnd);
		emiter->SetGravityVelocity(sm::Vec3(0, 200, 0));
		emiter->SetColorOverLifetime(sm::Vec4(1, 1, 1, ee->m_opacityStart), sm::Vec4(1, 1, 1, ee->m_opacityEnd));
		emiter->SetSpeedOverLifetime(ee->m_speedStart, ee->m_speedEnd);
		emiter->SetSourceDirection(direction.GetNormalized(), 0.1f);
		
		emiter->SetSourceAsPlane(ee->m_position, ee->m_planeBase1, ee->m_planeBase2);

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

