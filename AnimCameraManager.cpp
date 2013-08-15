#include "AnimCameraManager.h"
#include <algorithm>
#include <fstream>

AnimCameraManager::AnimCameraManager(void)
{
	camIndex = 0;
}

AnimCameraManager::~AnimCameraManager(void)
{
}

void AnimCameraManager::Load(const std::string &path, Animation *anim)
{
	std::ifstream file(path, std::ios_base::binary);
	file.seekg(0, std::ios::end);
	unsigned fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	char *buff = new char[fileSize];
	file.read(buff, fileSize);
	file.close();

	BinaryReader br(buff);

	int camCount = br.Read<int>();
	for (int i = 0; i < camCount; i++)
	{
		AnimCamera *cam = AnimCamera::FromStream(br);

		float animLen = anim->GetAnimLengthById(cam->GetId());
		cam->SetLastKeyFrameTime(animLen);

		anim->AssignTransformable(cam);

		cameras.push_back(cam);
	}

	delete [] buff;

	std::sort(cameras.begin(), cameras.end(), AnimCameraComparer());
}

AnimCamera *AnimCameraManager::GetActiveCamera(float time)
{
	while (camIndex < cameras.size() && time > cameras[camIndex]->GetLastKeyFrameTime())
		camIndex++;

	return cameras[min(camIndex, cameras.size() - 1)];
}
