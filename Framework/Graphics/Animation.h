#pragma once

#include <Math\Vec3.h>
#include <Math\Quat.h>
#include <Math\Matrix.h>
#include <vector>

#include "Interpolators/InterpolatorFactory.h"

#include "ITransformable.h"

class Model;
class Mesh;

class Animation
{
public:
	int id;
	std::string nodeName;

	sm::Matrix worldTMInv;

	sm::Vec3 localPos;
	sm::Quat localRot;
	sm::Vec3 localScale;

	IInterpolator<sm::Vec3> *pos;
	IInterpolator<sm::Quat> *rot;
	IInterpolator<sm::Vec3> *scale;

	int m_lastPosKeyframeIndex;
	int m_lastRotKeyframeIndex;
	int m_lastScaleKeyframeIndex;

	std::vector<Animation*> subAnims;

	ITransformable *mesh;

	bool hasOwnRotate;
	float ownAngle;
	float angleScale;

	float m_animLength;

public:
	Animation(void);
	~Animation(void);

	void AssignModel(Model *model);
	void AssignTransformable(ITransformable *transformable);
	void Update(float time, const sm::Matrix &transform, float seconds);

	Animation *GetAnimationById(int id);
	Animation *GetAnimationByNodeName(const std::string &name);
	float GetAnimLength();
	float GetAnimLengthById(int id);
};
