#include "AnimCameraManager.h"
#include <IO/Path.h>
#include <algorithm>

AnimCameraManager::AnimCameraManager(void)
{
	camIndex = 0;
}

AnimCameraManager::~AnimCameraManager(void)
{
}

void AnimCameraManager::Load(const std::string &path, Animation *anim)
{
	uint8_t *data;
	uint32_t size;
	Path::GetFileContent(path.c_str(), data, size);

	BinaryReader br(data);

	int camCount = br.Read<int>();
	for (int i = 0; i < camCount; i++)
	{
		AnimCamera *cam = AnimCamera::FromStream(br);

		if (anim != NULL)
		{
			float animLen = anim->GetAnimLengthById(cam->GetId());
			cam->SetLastKeyFrameTime(animLen);

			anim->AssignTransformable(cam);
		}

		cameras.push_back(cam);
	}
	
	delete [] data;

	std::sort(cameras.begin(), cameras.end(), AnimCameraComparer());
}

AnimCamera *AnimCameraManager::GetActiveCamera(float time)
{
	while (camIndex < cameras.size() && time > cameras[camIndex]->GetLastKeyFrameTime())
		camIndex++;

	return cameras[min(camIndex, cameras.size() - 1)];
}

AnimCamera *AnimCameraManager::GetCameraByName(const std::string &name)
{
	for (uint32_t i = 0; i < cameras.size(); i++)
		if (cameras[i]->GetName() == name)
			return cameras[i];

	return NULL;
}

