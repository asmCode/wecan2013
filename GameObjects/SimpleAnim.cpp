#include "SimpleAnim.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Animation.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

SimpleAnim::SimpleAnim(Animation *anim, Model *model, float hideBefore) :
	m_anim(anim),
	m_model(model),
	m_hideBefore(hideBefore)
{
	vis = false;

	assert(m_anim != NULL);
	assert(m_model != NULL);

	m_anim->AssignModel(m_model);

	for (int i = 0; i < m_model->m_meshParts.size(); i++)
	{
		m_model->m_meshParts[i]->m_alwaysHide = true;
	}
}

void SimpleAnim::Awake()
{
}

void SimpleAnim::Update(float time, float seconds)
{
	if (time > m_hideBefore && !vis)
	{
		vis = true;
		for (int i = 0; i < m_model->m_meshParts.size(); i++)
		{
			m_model->m_meshParts[i]->m_alwaysHide = false;
		}
	}


	m_anim->Update(time, sm::Matrix::IdentityMatrix(), seconds);
}

void SimpleAnim::Draw()
{
}

std::vector<MeshPart*>& SimpleAnim::GetMeshParts()
{
	return m_model->m_meshParts;
}

