#pragma once

#include "../GameObject.h"

class Model;
class Animation;

class SimpleAnim : public GameObject
{
public:
	SimpleAnim(Animation *anim, Model *model, float hideBefore);

	virtual void Awake();

	virtual void Update(float time, float seconds);
	virtual void Draw();

	std::vector<MeshPart*>& GetMeshParts();

	virtual void ClearLightmaps();

protected:
	Model *m_model;
	Animation *m_anim;

	bool vis;
	float m_hideBefore;
};

