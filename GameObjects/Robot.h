#pragma once

#include "../GameObject.h"
#include <vector>

class Model;
class Mesh;
class Animation;
class CreditsDance;

class Robot : public GameObject
{
public:
	~Robot();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

	void SetCreditsDance(CreditsDance *creditsDanceObject);

private:
	enum Actions
	{
		Actions_Releasing = 0,
		Actions_Running,
		Actions_Greetz,
		Actions_BreakingWall,
		Actions_Dancing,
		Actions_Count
	};

	Animation *m_bindPose;
	Animation *m_testWalk;
	Animation *m_creditsDance;
	Animation *m_currentAnimation;
	Animation *m_throwWheelAnim;
	Animation *m_writtingAnim;
	Animation *m_goToPostersAnim;

	Model *m_goToPostersModel;

	CreditsDance *m_creditsDanceObject;

	std::vector<MeshPart*> m_allMeshParts;

	Model *m_robot;
};

