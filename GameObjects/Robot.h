#pragma once

#include "../GameObject.h"
#include <vector>

class Model;
class Mesh;
class Animation;
class RobotElement;

class Robot : public GameObject
{
public:
	~Robot();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	std::vector<MeshPart*>& GetMeshParts();

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

	enum BodyIndices
	{
		Head = 0,
		LFoot,
		RFoot,
		LShin,
		RShin,
		LThigh,
		RThigh,

		BodyIndicesCount
	};

	RobotElement **m_robotElements;

	Animation *m_bindPose;
	Animation *m_testWalk;
	Animation *m_currentAnimation;

	std::vector<MeshPart*> m_allMeshParts;

	Model *m_robot;

	std::string m_robotElementNamesMapping[BodyIndicesCount];
};

