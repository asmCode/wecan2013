#pragma once

#include "../GameObject.h"

class Model;
class Animation;

class SimpleAnim : public GameObject
{
public:
	SimpleAnim(Animation *anim, Model *model, float hideBefore);

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

private:
	Model *m_model;
	Animation *m_anim;

	bool vis;
	float m_hideBefore;
};

