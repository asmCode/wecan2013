#include "TrackClip.h"

#include <Graphics/Animation.h>
#include <assert.h>

TrackClip::TrackClip(Animation *bindPose, Animation *clip, Animation *trackAnim) :
	m_bindPose(bindPose),
	m_clip(clip),
	m_trackAnim(trackAnim),
	m_lastPosition(0, 0, 0),
	m_runTime(0.0f)
{
	assert(m_bindPose != NULL);
	assert(m_clip != NULL);
	assert(trackAnim != NULL);

	m_runLenght = m_clip->GetAnimLength();
}

void TrackClip::Activate()
{
	m_bindPose->ReplaceAnimation(m_clip);
}

void TrackClip::Deactivate()
{
}

void TrackClip::Update(float time, float seconds)
{
	m_trackAnim->Update(time, sm::Matrix::IdentityMatrix(), seconds);

	sm::Vec3 curPos = m_trackAnim->subAnims[0]->m_currentNodeTransform * sm::Vec3(0, 0, 0);
	sm::Vec3 dir = curPos - m_lastPosition;

	dir.y = 0.0f;
	float dirLength = dir.GetLength();
	dir.Normalize();
	m_lastPosition = curPos;
	sm::Vec3 right = (dir * sm::Vec3(0, 1, 0)).GetNormalized();
	sm::Vec3 up = (right * dir).GetNormalized();

	sm::Matrix m = sm::Matrix::TranslateMatrix(curPos) * sm::Matrix::CreateLookAt(dir, up);

	m_runTime += (m_runLenght * (dirLength / 412.0f))  * 0.4f; // 412 is distance from oryginal animation
	
	if (m_runTime >= m_runLenght)
	{
	//	m_runTime -= m_runLenght;
		m_bindPose->ClearLastKeys();
	}

	m_runTime = fmod(m_runTime, m_runLenght);

	m_bindPose->Update(m_runTime, m, seconds);
}

float TrackClip::GetLastKeyframeTime()
{
	return m_trackAnim->GetAnimLength();
}
