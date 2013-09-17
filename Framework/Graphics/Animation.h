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

	// matrix is updated every time when SetAnimationTime(); is called
	sm::Matrix m_currentNodeTransform;

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

	std::vector<Animation*> *m_flattenedChilds;
	void FlattenChilds(std::vector<Animation*> &flattenChilds);

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

	// podmienia keyframy i transformacje ze zrodlowej animacji.
	// taki troche hack. przydatne do bindowania postaci do szkieletu.
	// Po zbindowaniu mozna wsadzac dowolna inna animacje.
	void ReplaceAnimation(Animation *sourceAnim);

	void SetAnimationTime(float time, const sm::Matrix &parentTransform);
	Animation *FindAnimationChild(const std::string &nodeName);
};
