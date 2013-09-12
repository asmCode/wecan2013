#pragma once

#include "../GameObject.h"

class Model;

class ShadowmapTest : public GameObject
{
public:
	~ShadowmapTest();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

private:
	Model *m_shadowmapTestModel;
};

