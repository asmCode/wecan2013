#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	for (int i = 0; i < (int)meshes.size(); i++)
		delete meshes[i];

	meshes.clear();
}

std::vector<Mesh*> &Model::GetMeshes()
{
	return meshes;
}

void Model::GetMeshParts(std::vector<MeshPart*> &meshParts)
{
	for (unsigned i = 0; i < meshes.size(); i++)
		for (unsigned j = 0; j < meshes[i]->GetMeshParts().size(); j++)
			meshParts.push_back(meshes[i]->GetMeshParts()[j]);
}

Mesh* Model::FindMesh(const std::string &meshName)
{
	for (unsigned i = 0; i < meshes.size(); i++)
		if (meshes[i]->name == meshName)
			return meshes[i];

	return NULL;
}

void Model::SetTransformForMeshes(const sm::Matrix &transform)
{
	for (unsigned i = 0; i < meshes.size(); i++)
		meshes[i]->Transform() = transform;
}

void Model::SetAlwaysVisible(bool visible)
{
	for (unsigned i = 0; i < meshes.size(); i++)
		for (unsigned j = 0; j < meshes[i]->GetMeshParts().size(); j++)
			meshes[i]->GetMeshParts()[j]->IsAlvaysVisible() = visible;
}

Model *Model::CreateReference()
{
	Model *model = new Model();

	for (unsigned i = 0; i < meshes.size(); i++)
		model->meshes.push_back(meshes[i]->CreateReference());

	return model;
}

