#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>
#include <string>
#include <map>
#include <stdint.h>

class DemoController;
class ParticleEmmiter;
class DistortParticleHandler;
class Texture;
class Model;
class Shader;

class ParticlesManager
{
public:
	ParticlesManager();
	~ParticlesManager();

	void Initialize(Model *modelWithParticles);

	void SetViewMatrix(const sm::Matrix &viewMatrix);
	void SetProjMatrix(const sm::Matrix &viewMatrix);

	void Update(float seconds);
	void Draw();

private:
	class ParticlesSource
	{
	public:
		sm::Vec3 m_position;
		sm::Vec3 m_direction;
		sm::Vec3 m_planeBase1;
		sm::Vec3 m_planeBase2;
		float m_speedStart;
		float m_speedEnd;
		float m_sizeStart;
		float m_sizeEnd;
		float m_opacityStart;
		float m_opacityEnd;
		uint32_t m_particlesPersSecond;
		float m_lifeTimeMin;
		float m_lifeTimeMax;
	};

	sm::Matrix m_viewMatrix;
	sm::Matrix m_projMatrix;

	std::vector<ParticlesSource*> m_particlesSources;

	DemoController *m_demo;
	DistortParticleHandler *m_distortParticleHandler;

	std::vector<ParticleEmmiter*> m_emmiters;

	bool ParseName(const std::string &meshName, std::string &id, std::string &type);
};

