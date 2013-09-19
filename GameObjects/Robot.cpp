#include "Robot.h"
#include "CreditsDance.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <Math/MathUtils.h>
#include <algorithm>
#include <assert.h>

class AnimationClipComparer
{
public:
	bool operator() (Animation *c1, Animation *c2)
	{
		return c1->GetAnimLength() < c2->GetAnimLength();
	}
};

Robot::Robot() :
	m_activeClipIndex(0),
	m_lastClipIndex(-1)
{
}

Robot::~Robot()
{
}

void Robot::Awake()
{
	m_lastRunPosition = sm::Vec3(0, 0, 0);

	Content *content = demo->m_content;

	m_robot = content->Get<Model>("bind_pose");
	assert(m_robot != NULL);
	//m_robot->m_baseTransform = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);

	m_bindPose = content->Get<Animation>("bind_pose");
	assert(m_bindPose != NULL);

	m_run01Trajectory = content->Get<Animation>("run01");
	assert(m_run01Trajectory != NULL);
	
	const char* animNames[] =
	{
		"writting",
		"go_to_posters",
		"back_to_cigarette",
		"whisky",
		"cigarette",
		"angry",
		"porazenie",
		"throw",
		"run"
	};

	for (uint32_t i = 0; i < 9; i++)
	{
		Animation *anim = content->Get<Animation>(animNames[i]);
		assert(anim != NULL);

		Model *model = content->Get<Model>(animNames[i]);
		assert(model != NULL);

		m_allMeshParts.insert(m_allMeshParts.end(), model->m_meshParts.begin(), model->m_meshParts.end());
		
		for (uint32_t j = 0; j < model->m_meshParts.size(); j++)
			model->m_meshParts[j]->m_alwaysHide = true;

		anim->AssignModel(model);

		m_clips.push_back(anim);
		m_clipsModels.push_back(model);

		if (i <= 6) // ass stuck to chair
		{
			/////m_bindPose->ReplaceAnimation(m_goToPostersAnim);

			Animation *bindPoseHip = m_bindPose->FindAnimationChild("Hip", true);
			assert(bindPoseHip != NULL);
			Animation *wheelAnimation = anim->FindAnimationChild("futuris858", true);
			if (wheelAnimation == NULL)
				wheelAnimation = anim->FindAnimationChild("rama wozka", true);
			assert(wheelAnimation);
			wheelAnimation->subAnims.push_back(bindPoseHip);
		}
	}

	m_runTimeLEngth = m_clips[8]->GetAnimLength();
	m_runTime = 0.0f;

	//std::sort(m_clips.begin(), m_clips.end(), AnimationClipComparer());

	m_allMeshParts.insert(m_allMeshParts.begin(), m_robot->m_meshParts.begin(), m_robot->m_meshParts.end());
	//m_allMeshParts.insert(m_allMeshParts.begin(), m_goToPostersModel->m_meshParts.begin(), m_goToPostersModel->m_meshParts.end());

	m_bindPose->AssignModel(m_robot);
	/////m_bindPose->ReplaceAnimation(m_goToPostersAnim);

	//////m_goToPostersAnim->AssignModel(m_goToPostersModel);

	/////Animation *hip = m_bindPose->FindAnimationChild("Hip", true);

	/////Animation *wheelAnimation = m_goToPostersAnim->FindAnimationChild("futuris858", true);
	/////wheelAnimation->subAnims.push_back(hip);
	
	/////m_bindPose = m_goToPostersAnim;



	/////////
	m_bindPose->ReplaceAnimation(m_clips[8]);
}

void Robot::Update(float time, float seconds)
{
	/*
	while (m_activeClipIndex < m_clips.size() && time > m_clips[m_activeClipIndex]->GetAnimLength())
		m_activeClipIndex++;

	if (m_activeClipIndex >= m_clips.size())
		return;

	if (m_lastClipIndex != m_activeClipIndex)
	{
		if (m_lastClipIndex != -1)
		{
			for (uint32_t i = 0; i < m_clipsModels[m_lastClipIndex]->m_meshParts.size(); i++)
				m_clipsModels[m_lastClipIndex]->m_meshParts[i]->m_alwaysHide = true;
		}

		m_lastClipIndex = m_activeClipIndex;

		for (uint32_t i = 0; i < m_clipsModels[m_activeClipIndex]->m_meshParts.size(); i++)
				m_clipsModels[m_activeClipIndex]->m_meshParts[i]->m_alwaysHide = false;

		Animation *clip = m_clips[MathUtils::Min(m_activeClipIndex, m_clips.size() - 1)];
		m_bindPose->ReplaceAnimation(clip);
		m_activeAnimation = clip;
	}

	if (m_activeClipIndex == 7)
	{
		// this is for wheel
		m_bindPose->Update(time, sm::Matrix::IdentityMatrix(), seconds);
	}

	if (m_activeClipIndex == 8)
	{
		m_run01Trajectory->Update(time, sm::Matrix::IdentityMatrix(), seconds);

		m_runTime += seconds;
		m_runTime = fmod(m_runTime, m_runTimeLEngth);

		m_activeAnimation->Update(m_runTime, sm::Matrix::IdentityMatrix(), seconds);
	}
	else
	{
		//time = m_creditsDanceObject->GetAnimTime();
		m_activeAnimation->Update(time, sm::Matrix::IdentityMatrix(), seconds);
	}
	*/

	m_run01Trajectory->Update(time * 0.5f, sm::Matrix::IdentityMatrix(), seconds);

	sm::Vec3 curPos = m_run01Trajectory->subAnims[0]->m_currentNodeTransform * sm::Vec3(0, 0, 0);
	sm::Vec3 dir = curPos - m_lastRunPosition;

	dir.y = 0.0f;
	float dirLength = dir.GetLength();
	dir.Normalize();
	m_lastRunPosition = curPos;
	sm::Vec3 right = (dir * sm::Vec3(0, 1, 0)).GetNormalized();
	sm::Vec3 up = (right * dir).GetNormalized();

	sm::Matrix m = sm::Matrix::TranslateMatrix(curPos) * sm::Matrix::CreateLookAt(dir, up);

	m_runTime += (m_runTimeLEngth * (dirLength / 412.0f));
	
	if (m_runTime >= m_runTimeLEngth)
	{
		m_runTime -= m_runTimeLEngth;
		m_bindPose->ClearLastKeys();
	}

	//m_runTime = fmod(m_runTime, m_runTimeLEngth);

	m_bindPose->Update(m_runTime, m, seconds);
}

void Robot::Draw()
{
}

std::vector<MeshPart*>& Robot::GetMeshParts()
{
	return m_allMeshParts;
}

void Robot::SetCreditsDance(CreditsDance *creditsDanceObject)
{
	m_creditsDanceObject = creditsDanceObject;
}

