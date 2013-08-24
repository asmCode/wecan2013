#pragma once

#include <Math/Matrix.h>
#include <Graphics/ITransformable.h>
#include <IO\BinaryReader.h>
#include <Graphics/Interpolators/IInterpolator.h>
#include <Graphics/Interpolators/InterpolatorFactory.h>
#include "ICamera.h"

class AnimCamera : public ITransformable, public ICamera
{
private:
	int id;
	sm::Matrix view;
	sm::Matrix dummy;

	float fov;
	float trgDist;
	IInterpolator<float> *fovAnim;
	IInterpolator<float> *distAnim;
	float lastKeyFrameTime;

public:
	AnimCamera(int id);
	~AnimCamera(void);

	static AnimCamera* FromStream(BinaryReader &br);

	void SetLastKeyFrameTime(float time);
	float GetLastKeyFrameTime();
	bool IsCam();
	int GetId();
	sm::Matrix& Transform();
	sm::Matrix& AnimTransform();

	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();

	float GetFov(float time);
	float GetTargetDistance(float time);
};
