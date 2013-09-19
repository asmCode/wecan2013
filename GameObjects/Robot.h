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

	Animation *m_writtingAnim;
	Animation *m_goToPostersAnim;
	Animation *m_backToCigaretteAnim;
	Animation *m_whiskyAnim;
	Animation *m_cigaretteAnim;
	Animation *m_angryAnim;
	Animation *m_porazenieAnim;
	Animation *m_throwAnim;

	Animation *m_run01Trajectory;

	std::vector<Animation*> m_clips;
	std::vector<Model*> m_clipsModels;
	uint32_t m_activeClipIndex;
	uint32_t m_lastClipIndex;
	Animation *m_activeAnimation;

	Model *m_goToPostersModel;

	CreditsDance *m_creditsDanceObject;

	sm::Vec3 m_lastRunPosition;

	std::vector<MeshPart*> m_allMeshParts;

	Model *m_robot;

	float m_runTime;
	float m_runTimeLEngth;
};

