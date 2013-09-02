#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>
#include <string>
#include <map>

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
	};

	sm::Matrix m_viewMatrix;
	sm::Matrix m_projMatrix;

	std::map<std::string, ParticlesSource*> m_particlesSources;

	DemoController *m_demo;
	DistortParticleHandler *m_distortParticleHandler;

	std::vector<ParticleEmmiter*> m_emmiters;

	bool ParseName(const std::string &meshName, std::string &id, std::string &type);
};

