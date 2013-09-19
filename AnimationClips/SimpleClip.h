#pragma once

#include "IAnimationClip.h"
#include <stdint.h>

class Animation;

class SimpleClip : public IAnimationClip
{
public:
	SimpleClip(Animation *bindPose, Animation *clip);

	void Activate();
	void Deactivate();

	void Update(float time, float seconds);

	float GetLastKeyframeTime();

	void GetMeshParts(std::vector<MeshPart*> &meshParts) {};

private:
	Animation *m_bindPose;
	Animation *m_clip;
};

