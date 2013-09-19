#include "SimpleClip.h"

#include <Graphics/Animation.h>
#include <assert.h>

SimpleClip::SimpleClip(Animation *bindPose, Animation *clip) :
	m_bindPose(bindPose),
	m_clip(clip)
{
	assert(m_bindPose != NULL);
	assert(m_clip != NULL);
}

void SimpleClip::Activate()
{
	m_bindPose->ReplaceAnimation(m_clip);
}

void SimpleClip::Deactivate()
{
}

void SimpleClip::Update(float time, float seconds)
{
	m_clip->Update(time, sm::Matrix::IdentityMatrix(), seconds);
}

float SimpleClip::GetLastKeyframeTime()
{
	return m_clip->GetAnimLength();
}
