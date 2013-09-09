#include "GameObject.h"

std::vector<MeshPart*> GameObject::m_dummy;

std::vector<MeshPart*>& GameObject::GetMeshParts()
{
	return m_dummy;
}

