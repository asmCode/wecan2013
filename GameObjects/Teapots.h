#pragma once

#include "../GameObject.h"

class Model;

class Teapots : public GameObject
{
public:
	~Teapots();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

private:
	Model *m_teapotsModel;
};

