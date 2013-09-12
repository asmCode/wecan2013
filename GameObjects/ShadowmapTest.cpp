#include "ShadowmapTest.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

ShadowmapTest::~ShadowmapTest()
{
}

void ShadowmapTest::Awake()
{
	m_shadowmapTestModel = demo->m_content->Get<Model>("shadowmapping_test");
	assert(m_shadowmapTestModel != NULL);

	for (uint32_t i = 0; i < m_shadowmapTestModel->m_meshParts.size(); i++)
	{
		m_shadowmapTestModel->m_meshParts[i]->mesh->Transform() = sm::Matrix::ScaleMatrix(0.1f, 0.1f, 0.1f);
	}
}

void ShadowmapTest::Update(float time, float seconds)
{
}

void ShadowmapTest::Draw()
{
}

std::vector<MeshPart*>& ShadowmapTest::GetMeshParts()
{
	return m_shadowmapTestModel->m_meshParts;
}

