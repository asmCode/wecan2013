#include "WheelClip.h"

#include <Graphics/Animation.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Model.h>
#include <assert.h>

WheelClip::WheelClip(Animation *bindPose, Animation *clip, Model *model) :
	m_bindPose(bindPose),
	m_clip(clip),
	m_model(model)
{
	assert(m_bindPose != NULL);
	assert(m_clip != NULL);
	assert(m_model != NULL);

	m_clip->AssignModel(m_model);

	Animation *bindPoseHip = m_bindPose->FindAnimationChild("Hip", true);
	assert(bindPoseHip != NULL);

	Animation *wheelAnimation = m_clip->FindAnimationChild("futuris858", true);
	if (wheelAnimation == NULL)
		wheelAnimation = m_clip->FindAnimationChild("rama wozka", true);
	assert(wheelAnimation);

	wheelAnimation->subAnims.push_back(bindPoseHip);
}

void WheelClip::Activate()
{
	for (uint32_t i = 0; i < m_model->m_meshParts.size(); i++)
		m_model->m_meshParts[i]->m_alwaysHide = false;

	m_bindPose->ReplaceAnimation(m_clip);
}

void WheelClip::Deactivate()
{
	for (uint32_t i = 0; i < m_model->m_meshParts.size(); i++)
		m_model->m_meshParts[i]->m_alwaysHide = true;
}

void WheelClip::Update(float time, float seconds)
{
	m_clip->Update(time, sm::Matrix::IdentityMatrix(), seconds);
}

float WheelClip::GetLastKeyframeTime()
{
	return m_clip->GetAnimLength();
}

void WheelClip::GetMeshParts(std::vector<MeshPart*> &meshParts)
{
	meshParts.insert(meshParts.end(), m_model->m_meshParts.begin(), m_model->m_meshParts.end());
}
