#include "AnimCamera.h"

AnimCamera::AnimCamera(int id)
{
	this->id = id;

	fov = 60.0f;
	trgDist = 10.0f;

	fovAnim = NULL;
	distAnim = NULL;
	lastKeyFrameTime = 0;
}

AnimCamera::~AnimCamera(void)
{
	if (fovAnim != NULL)
		delete fovAnim;

	if (distAnim != NULL)
		delete distAnim;
}

sm::Matrix& AnimCamera::Transform()
{
	return view;
}

sm::Matrix& AnimCamera::AnimTransform()
{
	return dummy;
}

bool AnimCamera::IsCam()
{
	return true;
}

int AnimCamera::GetId()
{
	return id;
}

AnimCamera* AnimCamera::FromStream(BinaryReader &br)
{
	int id = br.Read<int>();

	AnimCamera *animCam = new AnimCamera(id);
	bool hasFovAnim = br.Read<bool>();
	if (hasFovAnim)
	{
		animCam->fovAnim = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_TCB);

		int keysCount = br.Read<int>();
		for (int i = 0; i < keysCount; i++)
		{
			float time = br.Read<float>();
			float val = br.Read<float>();
			animCam->fovAnim->AddKeyframe(time, val, false);
		}
	}
	else
	{
		animCam->fov = br.Read<float>();
	}

	bool hasDistAnim = br.Read<bool>();
	if (hasDistAnim)
	{
		animCam->distAnim = InterpolatorFactory::CreateInterpolator<float>(InterpolatorFactory::InterpolationMethod_TCB);

		int keysCount = br.Read<int>();
		for (int i = 0; i < keysCount; i++)
		{
			float time = br.Read<float>();
			float val = br.Read<float>();
			animCam->distAnim->AddKeyframe(time, val, false);
		}
	}
	else
	{
		animCam->trgDist = br.Read<float>();
	}

	return animCam;
}

float AnimCamera::GetFov(float time)
{
	if (fovAnim == NULL)
		return fov;
	else
	{
		float val;
		fovAnim->GetValue(time, val);
		return val;
	}
}

float AnimCamera::GetTargetDistance(float time)
{
	if (distAnim == NULL)
		return trgDist;
	else
	{
		float val;
		distAnim->GetValue(time, val);
		return val;
	}
}

float AnimCamera::GetLastKeyFrameTime()
{
	return lastKeyFrameTime;
}

void AnimCamera::SetLastKeyFrameTime(float time)
{
	this->lastKeyFrameTime = time;
}

sm::Matrix AnimCamera::GetViewMatrix()
{
	return view;
}

sm::Vec3 AnimCamera::GetPosition()
{
	return Transform().GetInversed() * sm::Vec3(0, 0, 0);
}
