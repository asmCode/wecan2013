#pragma once

#include "../GameObject.h"

class Model;

class Factory : public GameObject
{
public:
	~Factory();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

private:
	Model *m_factoryModel;
};

