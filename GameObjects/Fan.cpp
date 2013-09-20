#include "Fan.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

Fan::~Fan()
{
}

void Fan::Awake()
{
	m_fanModel = demo->m_content->Get<Model>("fan");
	assert(m_fanModel != NULL);

	angle = 0.0f;
}

void Fan::Update(float time, float seconds)
{
	if (time < 150)
	{
	angle += 4.0f * seconds;

	for (int i = 0; i < m_fanModel->meshes.size(); i++)
	{
		m_fanModel->meshes[i]->transform =
			m_fanModel->meshes[i]->m_worldMatrix *
			sm::Matrix::RotateAxisMatrix(angle, 1, 0, 0) *
			m_fanModel->meshes[i]->m_worldInverseMatrix;
	}
	}
	else
	{
		for (int i = 0; i < m_fanModel->meshes.size(); i++)
	{
		m_fanModel->meshes[i]->transform = sm::Matrix::IdentityMatrix();
	}
	}
}

void Fan::Draw()
{
}

std::vector<MeshPart*>& Fan::GetMeshParts()
{
	return m_fanModel->m_meshParts;
}

