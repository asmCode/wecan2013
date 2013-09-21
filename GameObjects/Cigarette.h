#pragma once

#include "SimpleAnim.h"

class Model;
class Mesh;
class Animation;
class ParticleEmmiter;
class DistortParticleHandler;

class Cigarette : public SimpleAnim
{
public:
	Cigarette(Animation *anim, Model *model, float hideBefore);

	void Awake();

	void Update(float time, float seconds);
	void DrawSmokes(float time, float seconds);

	std::vector<MeshPart*>& GetMeshParts();

	void ClearLightmaps();

private:
	Mesh *m_smokeSourceCig;
	Mesh *m_smokeSourceHead;
	Mesh *m_smokeSourceHeadTrg;

	ParticleEmmiter *m_particlesCig;
	ParticleEmmiter *m_particlesHead;

	DistortParticleHandler *m_distortParticleHandler;
};

