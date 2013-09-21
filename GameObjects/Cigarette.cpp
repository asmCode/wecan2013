#include "Cigarette.h"
#include "../DemoController.h"
#include "../Particles/ParticleEmmiter.h"
#include "../Particles/IParticleHandler.h"
#include "../DistortParticleHandler.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Animation.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

Cigarette::Cigarette(Animation *anim, Model *model, float hideBefore) :
	SimpleAnim(anim, model, hideBefore)
{
	m_smokeSourceCig = m_model->FindMesh("dym_papierosa");
	assert(m_smokeSourceCig != NULL);
	m_smokeSourceHead = m_model->FindMesh("glowa");
	assert(m_smokeSourceHead != NULL);
	m_smokeSourceHeadTrg = m_model->FindMesh("glowa_trg");
	assert(m_smokeSourceHeadTrg != NULL);

	Texture *smokeTexture = demo->m_content->Get<Texture>("smoke");
	assert(smokeTexture != NULL);

	Shader *distortionEffect = demo->m_content->Get<Shader>("DistortParticle");
	assert(distortionEffect != NULL);

	m_distortParticleHandler = new DistortParticleHandler(distortionEffect, smokeTexture, NULL);

	m_anim->Update(0, sm::Matrix::IdentityMatrix(), 1.0f);

	m_particlesCig = new ParticleEmmiter(100, m_distortParticleHandler);
	m_particlesCig->SetSparksPerSecond(20);
	m_particlesCig->SetLifeTime(0.5, 1);
	m_particlesCig->SetSizeOverLifetime(4, 50);
	m_particlesCig->SetGravityVelocity(sm::Vec3(0, 50, 0));
	m_particlesCig->SetColorOverLifetime(sm::Vec4(1, 1, 1, 0.5f), sm::Vec4(1, 1, 1, 0.0f));
	m_particlesCig->SetSpeedOverLifetime(0, 0);
	m_particlesCig->SetSourceDirection(sm::Vec3(0, 1, 0), 0.6f);

	m_particlesHead = new ParticleEmmiter(100, m_distortParticleHandler);
	m_particlesHead->SetSparksPerSecond(20);
	m_particlesHead->SetLifeTime(1, 2);
	m_particlesHead->SetSizeOverLifetime(20, 100);
	m_particlesHead->SetGravityVelocity(sm::Vec3(0, 100, 0));
	m_particlesHead->SetColorOverLifetime(sm::Vec4(1, 1, 1, 0.5f), sm::Vec4(1, 1, 1, 0.0f));
	m_particlesHead->SetSpeedOverLifetime(200, 0);
	m_particlesHead->SetSourceDirection(
		(m_smokeSourceHeadTrg->AnimTransform() * sm::Vec3(0, 0, 0) - m_smokeSourceHead->AnimTransform() * sm::Vec3(0, 0, 0)).GetNormalized(), 0.2f);
	m_particlesHead->SetSourcePosition(m_smokeSourceHead->AnimTransform() * sm::Vec3(0, 0, 0));

	for (int i = 0; i <  model->meshes.size(); i++)
	{
		model->meshes[i]->m_isShadowCaster = true;
	}
}

void Cigarette::ClearLightmaps()
{
	this->SimpleAnim::ClearLightmaps();
}

void Cigarette::Awake()
{
}

void Cigarette::Update(float time, float seconds)
{
	this->SimpleAnim::Update(time, seconds);

	m_particlesCig->SetViewMatrix(demo->m_view);
	m_particlesCig->SetProjMatrix(demo->m_proj);

	m_particlesHead->SetViewMatrix(demo->m_view);
	m_particlesHead->SetProjMatrix(demo->m_proj);

	m_particlesCig->SetSourcePosition(m_smokeSourceCig->AnimTransform() * sm::Vec3(0, 0, 0));

	m_particlesCig->Update(seconds);
	m_particlesHead->Update(seconds);
}

void Cigarette::DrawSmokes(float time, float seconds)
{
	m_particlesCig->Draw(seconds);

	if (time > 100 + (1817.0f / 4800.0f) && time < 106 + (1817.0f / 4800.0f))
		m_particlesHead->Draw(seconds);
}

std::vector<MeshPart*>& Cigarette::GetMeshParts()
{
	return m_model->m_meshParts;
}

