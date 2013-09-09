#include "Factory.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

Factory::~Factory()
{
}

void Factory::Awake()
{
	m_factoryModel = demo->m_content->Get<Model>("factory");
	assert(m_factoryModel != NULL);

	for (uint32_t i = 0; i < m_factoryModel->m_meshParts.size(); i++)
	{
		m_factoryModel->m_meshParts[i]->mesh->Transform() = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);
	}
}

void Factory::Update(float time, float seconds)
{
}

void Factory::Draw()
{
}

std::vector<MeshPart*>& Factory::GetMeshParts()
{
	return m_factoryModel->m_meshParts;
}

