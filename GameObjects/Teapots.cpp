#include "Teapots.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

Teapots::~Teapots()
{
}

void Teapots::Awake()
{
	m_teapotsModel = demo->m_content->Get<Model>("teapots");
	assert(m_teapotsModel != NULL);
}

void Teapots::Update(float time, float seconds)
{
}

void Teapots::Draw()
{
}

std::vector<MeshPart*>& Teapots::GetMeshParts()
{
	return m_teapotsModel->m_meshParts;
}

