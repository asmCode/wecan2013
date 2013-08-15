#include "GeometryBatch.h"

#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>

#include <assert.h>

GeometryBatch::GeometryBatch(void) :
	m_isVisible(true)
{
}

GeometryBatch::~GeometryBatch(void)
{
}

void GeometryBatch::AddModel(Model *model)
{
	assert(model != NULL);

	m_models.push_back(model);
	model->GetMeshParts(allMeshParts);
}

void GeometryBatch::SetVisibility(bool visible)
{
	m_isVisible = visible;
}

bool GeometryBatch::IsVisible() const
{
	return m_isVisible;
}

