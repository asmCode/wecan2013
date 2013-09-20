#include "Robot.h"
#include "CreditsDance.h"
#include <../AnimationClips/IAnimationClip.h>
#include <../AnimationClips/WheelClip.h>
#include <../AnimationClips/SimpleClip.h>
#include <../AnimationClips/TrackClip.h>
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
	Content *content = demo->m_content;

	m_robot = content->Get<Model>("bind_pose");
	assert(m_robot != NULL);
	//m_robot->m_baseTransform = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);

	m_bindPose = content->Get<Animation>("bind_pose");
	assert(m_bindPose != NULL);

	m_bindPose->AssignModel(m_robot);
	
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

	/*
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("writting"), content->Get<Model>("writting")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("go_to_posters"), content->Get<Model>("go_to_posters")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("back_to_cigarette"), content->Get<Model>("back_to_cigarette")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("whisky"), content->Get<Model>("whisky")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("cigarette"), content->Get<Model>("cigarette")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("angry"), content->Get<Model>("angry")));
	m_clips.push_back(new WheelClip(m_bindPose, content->Get<Animation>("porazenie"), content->Get<Model>("porazenie")));

	m_clips.push_back(new TrackClip(m_bindPose, content->Get<Animation>("run"), content->Get<Animation>("run01")));
	m_clips.push_back(new TrackClip(m_bindPose, content->Get<Animation>("run"), content->Get<Animation>("run02")));
	m_clips.push_back(new SimpleClip(m_bindPose, content->Get<Animation>("branch")));
	m_clips.push_back(new SimpleClip(m_bindPose, content->Get<Animation>("jump")));
	m_clips.push_back(new SimpleClip(m_bindPose, content->Get<Animation>("flip")));
	*/

	m_clips.push_back(new SimpleClip(m_bindPose, content->Get<Animation>("credits_dance")));

	for (uint32_t i = 0; i < m_clips.size(); i++)
	{
		m_clips[i]->Deactivate();
		m_clips[i]->GetMeshParts(m_allMeshParts);
	}

	m_allMeshParts.insert(m_allMeshParts.end(), m_robot->m_meshParts.begin(), m_robot->m_meshParts.end());
}

void Robot::Update(float time, float seconds)
{
	while (m_activeClipIndex < m_clips.size() && time > m_clips[m_activeClipIndex]->GetLastKeyframeTime())
		m_activeClipIndex++;

	if (m_activeClipIndex >= m_clips.size())
		return;

	if (m_lastClipIndex != m_activeClipIndex)
	{
		if (m_lastClipIndex != -1)
			m_clips[m_lastClipIndex]->Deactivate();

		m_clips[m_activeClipIndex]->Activate();

		m_lastClipIndex = m_activeClipIndex;
	}

	IAnimationClip *clip = m_clips[m_activeClipIndex];

	clip->Update(time, seconds);
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

