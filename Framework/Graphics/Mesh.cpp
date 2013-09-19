#include "Mesh.h"
#include "Model.h"
#include "Property.h"
#include "VertexP.h"
#include "VertexPN.h"
#include "VertexPCN.h"
#include "VertexPC2N.h"
#include "Material.h"
#include "MeshPart.h"

#include <Math\Matrix.h>

#include <gl/glew.h>


Mesh::Mesh()
{
	id = 0;
	colorMask = sm::Vec3(1, 1, 1);
	transform = sm::Matrix::IdentityMatrix();
	animTransform = sm::Matrix::IdentityMatrix();
	model = NULL;

	m_isShadowCaster = false;
	m_isShadowReceiver = false;
}

Mesh::~Mesh()
{
	for (int i = 0; i < (int)meshParts.size(); i++)
		delete meshParts[i];

	meshParts.clear();
}

std::vector<MeshPart*>& Mesh::GetMeshParts()
{
	return meshParts;
}

void Mesh::AddMeshPart( MeshPart *meshPart )
{
	meshParts.push_back(meshPart);
}

Property* Mesh::FindProperty(const std::string &name)
{
	std::map<std::string, Property*>::iterator it = m_properties.find(name);
	if (it != m_properties.end())
		return it->second;

	return NULL;
}

bool Mesh::IsCam()
{
	return false;
}

int Mesh::GetId()
{
	return id;
}

sm::Matrix& Mesh::Transform()
{
	return transform;
}

sm::Matrix& Mesh::AnimTransform()
{
	return animTransform;
}

sm::Matrix& Mesh::BaseTransform()
{
	return model->m_baseTransform;
}

Mesh *Mesh::CreateReference()
{
	Mesh *mesh = new Mesh();

	mesh->id = id;
	mesh->name = name;
	mesh->colorMask = colorMask;
	mesh->transform = transform;
	mesh->animTransform = animTransform;
	mesh->m_properties = m_properties;
	mesh->meshParts.insert(mesh->meshParts.begin(), meshParts.begin(), meshParts.end());

	return mesh;
}

void Mesh::Initialize()
{
	Property *shadowCasterProperty = FindProperty("castShadows");
	if (shadowCasterProperty != NULL)
		m_isShadowCaster = shadowCasterProperty->GetIntValue() == 1;

	Property *shadowReceiverProperty = FindProperty("receiveshadows");
	if (shadowReceiverProperty != NULL)
		m_isShadowReceiver = shadowReceiverProperty->GetIntValue() == 1;
}

