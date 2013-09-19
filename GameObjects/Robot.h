#pragma once

#include "../GameObject.h"
#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>
#include <stdint.h>

class Model;
class Mesh;
class Animation;
class CreditsDance;
class IAnimationClip;

class Robot : public GameObject
{
public:
	Robot();
	~Robot();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

	void SetCreditsDance(CreditsDance *creditsDanceObject);

private:
	Animation *m_bindPose;
	Model *m_robot;

	std::vector<IAnimationClip*> m_clips;

	uint32_t m_activeClipIndex;
	uint32_t m_lastClipIndex;

	CreditsDance *m_creditsDanceObject;

	std::vector<MeshPart*> m_allMeshParts;
};

