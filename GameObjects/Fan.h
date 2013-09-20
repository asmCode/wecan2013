#pragma once

#include "../GameObject.h"

class Model;

class Fan : public GameObject
{
public:
	~Fan();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

private:
	Model *m_fanModel;

	float angle;
};

