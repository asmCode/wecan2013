#include "AnimCamera.h"
#include <IO/Path.h>

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

sm::Matrix& AnimCamera::BaseTransform()
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

	animCam->m_name = br.Read<std::string>();

	br.ReadBuffer(reinterpret_cast<char*>(animCam->view.a), sizeof(float) * 16);

	animCam->view = sm::Matrix::RotateAxisMatrix(MathUtils::PI2, 1, 0, 0) * animCam->view;

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

	animCam->m_nearClip = br.Read<float>();
	animCam->m_farClip = br.Read<float>();

	return animCam;
}

AnimCamera* AnimCamera::FromFile(const std::string &path)
{
	uint32_t size;
	uint8_t *data;
	if (!Path::GetFileContent(path.c_str(), data, size))
		return NULL;

	BinaryReader br(data);

	AnimCamera *cam = FromStream(br);

	delete [] data;

	return cam;
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

std::string AnimCamera::GetName() const
{
	return m_name;
}

float AnimCamera::GetNearClip()
{
	return m_nearClip;
}

float AnimCamera::GetFarClip()
{
	return m_farClip;
}

