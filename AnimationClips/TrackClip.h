#pragma once

#include "IAnimationClip.h"
#include <Math/Vec3.h>
#include <stdint.h>

class Animation;

class TrackClip : public IAnimationClip
{
public:
	TrackClip(Animation *bindPose, Animation *clip, Animation *trackAnim);

	void Activate();
	void Deactivate();

	void Update(float time, float seconds);

	float GetLastKeyframeTime();

	void GetMeshParts(std::vector<MeshPart*> &meshParts) {};

private:
	Animation *m_bindPose;
	Animation *m_clip;
	Animation *m_trackAnim;

	sm::Vec3 m_lastPosition;

	float m_runTime;
	float m_runLenght;
};

