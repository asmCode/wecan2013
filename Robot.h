#pragma once

#include <Math\Matrix.h>
#include <Graphics\Interpolators\StateInterpolator.h>

#include <vector>
#include <string>

class Model;
class Mesh;
class Animation;
class Content;
class RobotElement;

class Robot
{
public:
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
		LToe,
		RToe,
		LFoot,
		RFoot,
		LShin,
		RShin,
		/*ShoulderCenter,
		Spine,
		HipCenter,

		ShoulderLeft,
		ElbowLeft,
		WristLeft,
		HandLeft,

		ShoulderRight,
		ElbowRight,
		WristRight,
		HandRight,

		HipLeft,
		KneeLeft,
		AnkleLeft,
		FootLeft,

		HipRight,
		KneeRight,
		AnkleRight,
		FootRight,*/

		BodyIndicesCount
	};

	Robot(void);
	~Robot(void);

	bool Initialize(Content *content);

	void Update(float time, float seconds);
	void Draw(float time, float seconds);

	std::vector<Model*>& GetModels();

	void SetCurrentAction(Actions action);

	void SetWorldMatrix(const sm::Matrix &m);
	void SetViewProjMatrix(const sm::Matrix &viewProj);
	float GetActionAnimLength(Actions action) const;

private:
	RobotElement **m_robotElements;

	Model *m_robot;
	Animation *m_robotAnim;

	sm::Matrix m_viewProj;

	std::string m_robotElementNamesMapping[BodyIndicesCount];

	Animation *m_animation;

	sm::Matrix CalcBoneMatrix(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd, const sm::Vec3 &rightVector);
};

