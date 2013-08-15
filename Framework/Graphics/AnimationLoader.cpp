#include "AnimationLoader.h"
#include "Animation.h"
#include <IO\BinaryReader.h>
#include <IO\Path.h>

Animation* AnimationLoader::LoadFromFile(const std::string &path)
{
	uint8_t *data;
	uint32_t size;

	if (!Path::GetFileContent(path.c_str(), data, size))
		return NULL;

	BinaryReader br(data);

	Animation *anim = new Animation();

	int subAnimsCount = br.Read<int>();
	for (int i = 0; i < subAnimsCount; i++)
		anim ->subAnims.push_back(LoadAnimation(br));

	delete [] data;

	return anim;
}

Animation* AnimationLoader::LoadAnimation(BinaryReader &br)
{
	Animation *anim = new Animation();

	bool hasOwnAnim = false;
	float angleScale;

	anim ->id = br.Read<int>();
	anim ->nodeName = br.Read<std::string>();
	anim ->worldTMInv = LoadMatrix(br);
	anim ->pos = LoadVec3Anim(br, anim ->localPos);
	anim ->rot = LoadQuatAnim(br, anim ->localRot, hasOwnAnim, angleScale);
	anim ->scale = LoadVec3Anim(br, anim ->localScale);

	if (hasOwnAnim)
	{
		anim->hasOwnRotate = hasOwnAnim;
		anim->angleScale = angleScale;
	}

	int subAnimsCount = br.Read<int>();
	for (int i = 0; i < subAnimsCount; i++)
		anim ->subAnims.push_back(LoadAnimation(br));

	return anim;
}

IInterpolator<sm::Vec3>* AnimationLoader::LoadVec3Anim(BinaryReader &br, sm::Vec3 &localVec3)
{
	int keysCount = br.Read<int>();
	if (keysCount == 0)
	{
		localVec3.x = br.Read<float>();
		localVec3.y = br.Read<float>();
		localVec3.z = br.Read<float>();
		return NULL;
	}

	IInterpolator<sm::Vec3> *inter = InterpolatorFactory::CreateInterpolator<sm::Vec3>(
		InterpolatorFactory::InterpolationMethod_TCB);

	float time;
	sm::Vec3 pos;

	for (int i = 0; i < keysCount; i++)
	{
		time = br.Read<float>();
		pos.x = br.Read<float>();
		pos.y = br.Read<float>();
		pos.z = br.Read<float>();

		inter ->AddKeyframe(time, pos, false);
	}

	return inter;
}

IInterpolator<sm::Quat>* AnimationLoader::LoadQuatAnim(BinaryReader &br, sm::Quat &localQuat, bool &ownAnim, float &angleScale)
{
	ownAnim = false;
	angleScale = 1.0f;

	int keysCount = br.Read<int>();
	if (keysCount == 0)
	{
		float angle = -br.Read<float>();
		sm::Vec3 axis;
		axis.x = br.Read<float>();
		axis.y = br.Read<float>();
		axis.z = br.Read<float>();
		axis.Normalize();
		localQuat.RotateToQuat(angle, axis);
		return NULL;
	}

	IInterpolator<sm::Quat> *inter = InterpolatorFactory::CreateInterpolator<sm::Quat>(
		InterpolatorFactory::InterpolationMethod_TCB);

	float time;
	float angle;
	sm::Vec3 axis;

	sm::Quat q;
	q.RotateToQuat(0, 0, 0, 0);

	float lastTime = -1;

	for (int i = 0; i < keysCount; i++)
	{
		time = br.Read<float>();
		angle = -br.Read<float>();
		axis.x = br.Read<float>();
		axis.z = -br.Read<float>();
		axis.y = br.Read<float>();
		axis.Normalize();

		/*if (abs(angle) < 3.1415f)
		{
			sm::Quat rot;
			rot.RotateToQuat(angle, axis);

			if (i == 0)
				q = rot;
			else
				q = q * rot;

			q.Normalize();

			inter ->AddKeyframe(time, q, false);
		}
		else
		{
			if (lastTime != -1)
			{
				float angle_step = abs(angle) / 3.1415f;
				float time_step = (time - lastTime) / (abs(angle) / 3.1415f);
				float dupa = 0.0f;
				for (int j = 0; j < (int)(abs(angle) / 3.1415f); j++)
				{
					sm::Quat rot;
					rot.RotateToQuat(dupa, axis);
					dupa += angle_step;

					if (i == 0)
						q = rot;
					else
						q = q * rot;

					q.Normalize();

					inter ->AddKeyframe(time + time_step * j, q, false);
				}
			}
		}
		
		lastTime = time;

		*/

		sm::Quat rot;
		rot.RotateToQuat(angle, axis);

		if (abs(angle) > 7.0f)
		{
			ownAnim = true;
			angleScale = abs(angle) / (3.1415f * 8.0f);
		}

		if (i == 0)
			q = rot;
		else
			q = q * rot;

		q.Normalize();

		inter ->AddKeyframe(time, q, false);
	}

	return inter;
}

sm::Matrix AnimationLoader::LoadMatrix(BinaryReader &br)
{
	//[0] [4] [8]  [12]     [0]
	//[1] [5] [9]  [13]     [1]
	//[2] [6] [10] [14]     [2]
	//[3] [7] [11] [15]     [3]

	sm::Matrix m;
	m.a[0] = br.Read<float>();
	m.a[1] = br.Read<float>();
	m.a[2] = br.Read<float>();
	m.a[3] = br.Read<float>();
	m.a[4] = br.Read<float>();
	m.a[5] = br.Read<float>();
	m.a[6] = br.Read<float>();
	m.a[7] = br.Read<float>();
	m.a[8] = br.Read<float>();
	m.a[9] = br.Read<float>();
	m.a[10] = br.Read<float>();
	m.a[11] = br.Read<float>();
	m.a[12] = br.Read<float>();
	m.a[13] = br.Read<float>();
	m.a[14] = br.Read<float>();
	m.a[15] = br.Read<float>();
	return m;
}
