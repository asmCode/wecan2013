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
	std::string m_name;
	sm::Matrix view;
	sm::Matrix dummy;

	float fov;
	float trgDist;
	float m_nearClip;
	float m_farClip;
	IInterpolator<float> *fovAnim;
	IInterpolator<float> *distAnim;
	float lastKeyFrameTime;

public:
	AnimCamera(int id);
	~AnimCamera(void);

	static AnimCamera* FromStream(BinaryReader &br);
	static AnimCamera* FromFile(const std::string &path);

	void SetLastKeyFrameTime(float time);
	float GetLastKeyFrameTime();
	bool IsCam();
	int GetId();
	std::string GetName() const;
	sm::Matrix& Transform();
	sm::Matrix& AnimTransform();
	sm::Matrix& BaseTransform();
	float GetNearClip();
	float GetFarClip();

	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();

	float GetFov(float time);
	float GetTargetDistance(float time);
};
