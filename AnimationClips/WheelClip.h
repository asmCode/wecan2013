#pragma once

#include "IAnimationClip.h"
#include <stdint.h>

class Animation;
class Model;

class WheelClip : public IAnimationClip
{
public:
	WheelClip(Animation *bindPose, Animation *clip, Model *model);

	void Activate();
	void Deactivate();

	void Update(float time, float seconds);

	float GetLastKeyframeTime();

	void GetMeshParts(std::vector<MeshPart*> &meshParts);

private:
	Animation *m_bindPose;
	Animation *m_clip;
	Model *m_model;
};

